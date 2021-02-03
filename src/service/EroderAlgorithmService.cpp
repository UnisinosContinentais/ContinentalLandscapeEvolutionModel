#include "continental/landscapeevolutionmodel/service/EroderAlgorithmService.h"
#include <continental/datamanagement/Raster.h>
#include <continental/datamanagement/RasterFile.h>
#include "continental/hydrotools/service/HeuristicSinkRemovalUtil.h"
#include <continental/hydrotools/service/HeuristicSinkRemoval.h>
#include <continental/hydrotools/service/HeuristicSinkRemovalUtil.h>
#include <continental/hydrotools/service/FlowDirection.h>
#include "continental/landscapeevolutionmodel/service/HydroToolsAlgorithmService.h"
#include "continental/landscapeevolutionmodel/service/DirectionCalculatorService.h"
#include <QString>
#include <QDebug>
#include <memory>
#include <cmath>
#include <vector>
#include <iostream>

using namespace continental::datamanagement;
using namespace continental::hydrotools;
using namespace continental::landscapeevolutionmodel::domain;

namespace continental {
namespace landscapeevolutionmodel {
namespace service {

EroderAlgorithmService::EroderAlgorithmService()
{

}

EroderAlgorithmService::EroderAlgorithmService(std::shared_ptr<Raster<float>> initialGrid, double erodibility, size_t deltaT, double concavityIndex) :
    m_deltaT(deltaT),
    m_erodibility(erodibility),
    m_concavityIndex(concavityIndex),
    m_initialGrid(initialGrid)
{
	m_numberOfCols = m_initialGrid->getCols();
	m_numberOfRows = m_initialGrid->getRows();
	m_cellSize = m_initialGrid->getCellSize();
}


void EroderAlgorithmService::setFlowDirection(const std::shared_ptr<Raster<short>> flowDirection)
{
	m_flowDirection = flowDirection;
}

void EroderAlgorithmService::setFlowAccumulation(const std::shared_ptr<Raster<float>> flowAccumulation)
{
	m_flowAccumulation = flowAccumulation;
}

void EroderAlgorithmService::setStreamDefinition(const std::shared_ptr<Raster<short>> streamDefinition)
{
	m_streamDefinition = streamDefinition;
}

void EroderAlgorithmService::setStreamSegmentation(const std::shared_ptr<Raster<short>> streamSegmentation)
{
	m_streamSegmentation = streamSegmentation;
}

void EroderAlgorithmService::setCatchment(const std::shared_ptr<Raster<short>> catchment)
{
	m_catchment = catchment;
}


void EroderAlgorithmService::execute()
{
    // se os pontos estão lado a lado deltaL recebe este valor
	double horizontalDeltaL = m_cellSize;
	
	// se os pontos estão em diaginal, deltaL recebe este valor
	double diagonalDeltaL = sqrt(2 * m_initialGrid->getCellSize() * m_initialGrid->getCellSize()); 
	
	// vlores do Modelo de Elevação de Topografia 
    Raster<float> dem = *m_initialGrid;

	// valor de celula e canal de rio no stream definition
    short channelMarkerValue = 1;

    // varre todo o domínio computacional
    #pragma omp parallel for
    for (int iAux = 0; iAux <= static_cast<int>(m_numberOfRows) - 1; ++iAux)
	{
		size_t i = iAux;

		for (size_t j = 0; j <= m_numberOfCols - 1; ++j)
		{
			 // se a celula for de canal aplica o método			
			if (m_streamDefinition->getData(i, j) == channelMarkerValue)
			{
				// ao detectar que a celula é de canal, detecta para qual celula está esta sendo passado o fluxo
				size_t rowReceiver = i; 
				size_t colReceiver = j;

				

				bool result = moveToFlowDirection(m_flowDirection->getData(i, j), rowReceiver, colReceiver, m_numberOfRows, m_numberOfCols);

				// iniciliaza o valor topografico da que recebe
				double receiverElevation = 0.0;
				
				if (result)
				{
					// se a celula estiver passando fluxo para outra celula, atualiza o valor 
					receiverElevation = dem.getData(rowReceiver, colReceiver);
				}
				else
				{
					throw std::exception("Sem receiver.");
				}
				
                // deltaL é a distancia entre dois pontos consecutivos no domínio do canal
                double deltaL = 0.0;
                // Atualiza o valor de deltaL  em função da disposição geométrica entre os pontos
                if (rowReceiver == i || colReceiver == j)
				{
					deltaL = horizontalDeltaL;
				}
				else
				{
					deltaL = diagonalDeltaL;
				}
				// se o valor topografico for inferior a zero, recebe 0
				if (dem.getData(i, j) < 0.0 || qFuzzyCompare(dem.getData(i, j), 0.0f))
				{
					m_initialGrid->setData(i, j, 0);
				}
				else
				{
					// calculo da erosão  E = -k * A^m * S de uma celula para outra
                    auto value = ((-m_erodibility * pow((m_flowAccumulation->getData(i, j) * m_cellSize * m_cellSize), m_concavityIndex) * (dem.getData(i, j) - receiverElevation) / deltaL) * m_deltaT) + dem.getData(i, j);
					m_initialGrid->setData(i, j, value);

				}
			}
		}
    }
}

void EroderAlgorithmService::executeWithImplicitErosion(bool onlyMainDrainageNetwork, double drainageNetworksLenghtPercent, short facLimit)
{
    // se os pontos estão lado a lado deltaL recebe este valor
    double horizontalDeltaL = m_cellSize;
    // se os pontos estão em diaginal, deltaL recebe este valor
    double diagonalDeltaL = sqrt(2 * m_initialGrid->getCellSize() * m_initialGrid->getCellSize());
    
	// vlores do Modelo de Elevação de Topografia
	Raster<float> &dem = *m_initialGrid;

	// Executa o algoritmo de arvore para captação das diferentes redes de drenagem. true pega só a principal.
    DirectionCalculatorService directionCalculator(m_flowDirection, m_flowAccumulation);
    directionCalculator.setFacLimit(facLimit);
	directionCalculator.execute(onlyMainDrainageNetwork);

    std::vector<std::shared_ptr<DrainageNetwork>> &drainageNetworks = *directionCalculator.getDrainageNetworks();

    size_t totalPercent = (drainageNetworksLenghtPercent * drainageNetworks.size())/100;

    for (size_t auxi = 0; auxi < totalPercent; auxi++)
    {
        const std::shared_ptr<DrainageNetwork> &drainageNetwork = drainageNetworks[auxi];
		bool isFirst = true;

		for (PositionMatrix &position : drainageNetwork->positions)
        {
            double value = 0.0;
            double flowDirectionValue = m_flowDirection->getData(position.row, position.col);

            if (flowDirectionValue == DirectionCalculatorService::DIRECTION_DOWN || flowDirectionValue == DirectionCalculatorService::DIRECTION_LEFT
                    || flowDirectionValue == DirectionCalculatorService::DIRECTION_RIGHT || flowDirectionValue == DirectionCalculatorService::DIRECTION_UP)
            {
                value = (m_erodibility * std::pow(m_flowAccumulation->getData(position.row, position.col) * (m_cellSize * m_cellSize), m_concavityIndex) * m_deltaT) / horizontalDeltaL;
            }
            else
            {
                value = (m_erodibility * std::pow(m_flowAccumulation->getData(position.row, position.col) * (m_cellSize * m_cellSize), m_concavityIndex) * m_deltaT) / diagonalDeltaL;
            }

			if (!isFirst)
			{
				size_t rowReceiver = position.row;
				size_t colReceiver = position.col;
				moveToFlowDirection(flowDirectionValue, rowReceiver, colReceiver, m_numberOfRows, m_numberOfCols);
            
                dem.setData(position.row, position.col, (value * dem.getData(rowReceiver, colReceiver) + dem.getData(position.row, position.col)) / (value + 1.0));
			
			}
			
			isFirst = false;
        }
    }
}

bool EroderAlgorithmService::moveToFlowDirection(short value, size_t& row, size_t& column, size_t rows, size_t cols)
{
	//32	64	    128
    //16	0	    1      'Configuração das direções de fluxo para o IDRISI Kilimanjaro
	//	8	4	    2

	switch (value) //Identifica o sentido conforme o numero
	{
	case 128:
		if (row == 0)
		{
			return false;
		}
		row--;
		if (column + 1 == cols)
		{
			return false;
		}
		column++;
		return true;
	case 1:
		if (column + 1 == cols)
		{
			return false;
		}
		column++;
		return true;
	case 2:
		if (row + 1 == rows)
		{
			return false;
		}
		row++;
		if (column + 1 == cols)
		{
			return false;
		}
		column++;
		return true;
	case 4:
		if (row + 1 == rows)
		{
			return false;
		}
		row++;
		return true;
	case 8:
		if (row + 1 == rows)
		{
			return false;
		}
		row++;
		if (column == 0)
		{
			return false;
		}
		column--;
		return true;
	case 16:
		if (column == 0)
		{
			return false;
		}
		column--;
		return true;
	case 32:
		if (row == 0)
		{
			return false;
		}
		row--;
		if (column == 0)
		{
			return false;
		}
		column--;
		return true;
	case 64:
		if (row == 0)
		{
			return false;
		}
		row--;
		return true;
	}

	return false;
}


}
}
}
