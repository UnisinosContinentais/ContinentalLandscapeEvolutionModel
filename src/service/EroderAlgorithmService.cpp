#include "continental/landscapeevolutionmodel/service/EroderAlgorithmService.h"
#include <continental/datamanagement/Raster.h>
#include <continental/datamanagement/RasterFile.h>
#include "continental/hydrotools/service/HeuristicSinkRemovalUtil.h"
#include <continental/hydrotools/service/HeuristicSinkRemoval.h>
#include <continental/hydrotools/service/HeuristicSinkRemovalUtil.h>
#include <continental/hydrotools/service/FlowDirection.h>
#include "continental/landscapeevolutionmodel/domain/EnumDirection.h"
#include "continental/landscapeevolutionmodel/service/HydroToolsAlgorithmService.h"
#include "continental/landscapeevolutionmodel/service/DirectionCalculatorService.h"
#include <QString>
#include <QDebug>
#include <memory>
#include <cmath>
#include <vector>

using namespace continental::datamanagement;
using namespace continental::hydrotools;
using namespace continental::landscapeevolutionmodel::domain;

namespace continental {
namespace landscapeevolutionmodel {
namespace service {

EroderAlgorithmService::EroderAlgorithmService()
{

}

void EroderAlgorithmService::setFlowDirection(const std::shared_ptr<Raster<short>> flowDirection)
{
	m_flowDirection = flowDirection;
}

void EroderAlgorithmService::setFlowAccumulation(const std::shared_ptr<Raster<int>> flowAccumulation)
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

size_t EroderAlgorithmService::getNumberOfIterations() const
{
    return m_numberOfIterations;
}

void EroderAlgorithmService::setNumberOfIterations(const size_t &numberOfIterations)
{
    m_numberOfIterations = numberOfIterations;
}

double EroderAlgorithmService::getDimensionLessPrecipitationRate() const
{
    return m_dimensionLessPrecipitationRate;
}

void EroderAlgorithmService::setDimensionLessPrecipitationRate(double precipitationRate)
{
    m_dimensionLessPrecipitationRate = precipitationRate;
}

std::shared_ptr<datamanagement::Raster<double> > EroderAlgorithmService::getRaster() const
{
    return m_raster;
}

void EroderAlgorithmService::setRaster(const std::shared_ptr<datamanagement::Raster<double>> &raster)
{
    m_raster = raster;
    m_numberOfCols = m_raster->getCols();
    m_numberOfRows = m_raster->getRows();
    m_cellSize = m_raster->getCellSize();
}

double EroderAlgorithmService::getErodibility() const
{
    return m_erodibility;
}

void EroderAlgorithmService::setErodibility(double erodibility)
{
    m_erodibility = erodibility;
}

double EroderAlgorithmService::getConcavityIndex() const
{
    return m_concavityIndex;
}

void EroderAlgorithmService::setConcavityIndex(double concavityIndex)
{
    m_concavityIndex = concavityIndex;
}

double EroderAlgorithmService::getDimensionLessDepositionCoeficient() const
{
    return m_dimensionLessDepositionCoeficient;
}

void EroderAlgorithmService::setDimensionLessDepositionCoeficient(double dimensionLessDepositionCoeficient)
{
    m_dimensionLessDepositionCoeficient = dimensionLessDepositionCoeficient;
}

size_t EroderAlgorithmService::getDeltaTime() const
{
    return m_deltaTime;
}

void EroderAlgorithmService::setDeltaTime(const size_t &deltaT)
{
    m_deltaTime = deltaT;
}

void EroderAlgorithmService::useOnlyMainDrainageNetwork()
{
    m_drainageNetworkTypeLimit = OnlyMain;
}

void EroderAlgorithmService::useDrainageNetworkAmountLimit(size_t amountLimit)
{
    m_drainageNetworkTypeLimit = Amount;
    m_drainageNetworkAmountLimit = amountLimit;
}

void EroderAlgorithmService::useDrainageNetworkPercentLimit(double percentLimit)
{
    m_drainageNetworkTypeLimit = Percent;
    m_drainageNetworkPercentLimit = percentLimit;
}

int EroderAlgorithmService::getFlowAccumulationLimit() const
{
    return m_flowAccumulationLimit;
}

void EroderAlgorithmService::setFlowAccumulationLimit(int flowAccumulationLimit)
{
    m_flowAccumulationLimit = flowAccumulationLimit;
}

std::shared_ptr<datamanagement::Raster<double> > EroderAlgorithmService::getUplift() const
{
    return m_uplift;
}

void EroderAlgorithmService::setUplift(const std::shared_ptr<datamanagement::Raster<double> > &uplift)
{
    m_uplift = uplift;
}

std::vector<std::vector<double>> EroderAlgorithmService::donorsSummation(std::vector<PositionMatrix> &positions, bool addUplift)
{
    std::vector<std::vector<double>> donorsSummation(m_numberOfRows, std::vector<double>(m_numberOfCols));

    const short fdrNoDataValue = m_flowDirection->getNoDataValue();

    for (size_t iAux = positions.size(); iAux >= 1; iAux--)
    {
        size_t i = iAux - 1;

        const PositionMatrix &position = positions[i];
        const size_t row = position.row;
        const size_t col = position.col;

        const short fdrValue = m_flowDirection->getData(row, col);

        std::vector<std::tuple<size_t, size_t, short>> directions = {
            {row - 1, col + 1, static_cast<short>(EnumDirection::UpRight)},
            {row - 1, col    , static_cast<short>(EnumDirection::Up)},
            {row - 1, col - 1, static_cast<short>(EnumDirection::UpLeft)},
            {row    , col - 1, static_cast<short>(EnumDirection::Left)},
            {row + 1, col - 1, static_cast<short>(EnumDirection::DownLeft)},
            {row + 1, col    , static_cast<short>(EnumDirection::Down)},
            {row + 1, col + 1, static_cast<short>(EnumDirection::DownRight)},
            {row    , col + 1, static_cast<short>(EnumDirection::Right)}
        };

        if (fdrValue != fdrNoDataValue)
        {
            for (const std::tuple<size_t, size_t, short> &direction : directions)
            {
                const size_t directionFdrRow = std::get<0>(direction);
                const size_t directionFdrCol = std::get<1>(direction);
                const short directionFdrValue = std::get<2>(direction);

                if (fdrValue == directionFdrValue && m_flowDirection->getData(directionFdrRow, directionFdrCol) != fdrNoDataValue)
                {
                    donorsSummation[directionFdrRow][directionFdrCol] += m_raster->getData(row, col);
                }
            }
        }
    }

    for (size_t iAux = positions.size(); iAux > 0; iAux--)
    {
        const size_t i = iAux - 1;

        const PositionMatrix &position = positions[i];
        const size_t row = position.row;
        const size_t col = position.col;

        const short fdrValue = m_flowDirection->getData(row, col);

        std::vector<std::tuple<size_t, size_t, short >> directions = {
            {row - 1, col + 1, static_cast<short>(EnumDirection::UpRight)},
            {row - 1, col    , static_cast<short>(EnumDirection::Up)},
            {row - 1, col - 1, static_cast<short>(EnumDirection::UpLeft)},
            {row    , col - 1, static_cast<short>(EnumDirection::Left)},
            {row + 1, col - 1, static_cast<short>(EnumDirection::DownLeft)},
            {row + 1, col    , static_cast<short>(EnumDirection::Down)},
            {row + 1, col + 1, static_cast<short>(EnumDirection::DownRight)},
            {row    , col + 1, static_cast<short>(EnumDirection::Right)}
        };

        if (fdrValue != fdrNoDataValue)
        {
            for (const std::tuple<size_t, size_t, short> &direction : directions)
            {
                const size_t directionFdrRow = std::get<0>(direction);
                const size_t directionFdrCol = std::get<1>(direction);
                const short directionFdrValue = std::get<2>(direction);

                if (fdrValue == directionFdrValue && m_flowDirection->getData(directionFdrRow, directionFdrCol) != fdrNoDataValue)
                {
                    if (addUplift && m_uplift != nullptr)
                    {
                        donorsSummation[directionFdrRow][directionFdrCol] += donorsSummation[row][col] + m_uplift->getData(row, col) * m_deltaTime;
                    }
                    else
                    {
                        donorsSummation[directionFdrRow][directionFdrCol] += donorsSummation[row][col];
                    }
                }
            }
        }
    }

    return donorsSummation;
}

void EroderAlgorithmService::execute()
{
    // se os pontos estão lado a lado deltaL recebe este valor
	double horizontalDeltaL = m_cellSize;
	
	// se os pontos estão em diaginal, deltaL recebe este valor
    double diagonalDeltaL = sqrt(2 * m_raster->getCellSize() * m_raster->getCellSize());
	
	// vlores do Modelo de Elevação de Topografia 


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
                    receiverElevation = m_raster->getData(rowReceiver, colReceiver);
				}
				else
				{
                    throw std::exception("Without receiver.");
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
                double  auxCompareZero = 0.0f;
                if (m_raster->getData(i, j) < 0.0 || qFuzzyCompare(m_raster->getData(i, j), auxCompareZero))
				{
                    m_raster->setData(i, j, 0);
				}
				else
				{
					// calculo da erosão  E = -k * A^m * S de uma celula para outra
                    auto value = ((-m_erodibility * pow((m_flowAccumulation->getData(i, j) * m_cellSize * m_cellSize), m_concavityIndex) * (m_raster->getData(i, j) - receiverElevation) / deltaL) * m_deltaTime) + m_raster->getData(i, j);
                    m_raster->setData(i, j, value);

				}
			}
		}
    }
}

void EroderAlgorithmService::executeWithImplicitErosion()
{
    // se os pontos estão lado a lado deltaL recebe este valor
    double horizontalDeltaL = m_cellSize;
    // se os pontos estão em diaginal, deltaL recebe este valor
    double diagonalDeltaL = sqrt(2 * m_raster->getCellSize() * m_raster->getCellSize());
    
	// vlores do Modelo de Elevação de Topografia
    Raster<double> &dem = *m_raster;

	// Executa o algoritmo de arvore para captação das diferentes redes de drenagem. true pega só a principal.
    DirectionCalculatorService directionCalculator(m_flowDirection, m_flowAccumulation);
    directionCalculator.setFlowAccumulationLimit(m_flowAccumulationLimit);
    switch (m_drainageNetworkTypeLimit)
    {
        case OnlyMain:
            directionCalculator.useOnlyMainDrainageNetwork();
            break;
        case Amount:
            directionCalculator.useDrainageNetworkAmountLimit(m_drainageNetworkAmountLimit);
            break;
        case Percent:
            directionCalculator.useDrainageNetworkPercentLimit(m_drainageNetworkPercentLimit);
            break;
        default:
            throw std::runtime_error("The limit of the drainage networks has not been defined.");
    }
    directionCalculator.execute();

    std::shared_ptr<std::vector<std::shared_ptr<DrainageNetwork>>> drainageNetworks = directionCalculator.getDrainageNetworks();

    for (const std::shared_ptr<DrainageNetwork> &drainageNetwork : *drainageNetworks)
    {
		bool isFirst = true;

		for (PositionMatrix &position : drainageNetwork->positions)
        {
            double value = 0.0;
            double flowDirectionValue = m_flowDirection->getData(position.row, position.col);

            if (flowDirectionValue == EnumDirection::Down || flowDirectionValue == EnumDirection::Left
                    || flowDirectionValue == EnumDirection::Right || flowDirectionValue == EnumDirection::Up)
            {
                value = (m_erodibility * std::pow(m_flowAccumulation->getData(position.row, position.col) * (m_cellSize * m_cellSize), m_concavityIndex) * m_deltaTime) / horizontalDeltaL;
            }
            else
            {
                value = (m_erodibility * std::pow(m_flowAccumulation->getData(position.row, position.col) * (m_cellSize * m_cellSize), m_concavityIndex) * m_deltaTime) / diagonalDeltaL;
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

void EroderAlgorithmService::executeWithErosionDeposition()
{
    // Executa o algoritmo de arvore para captação das diferentes redes de drenagem. true pega só a principal.
    DirectionCalculatorService directionCalculator(m_flowDirection, m_flowAccumulation);
    directionCalculator.setFlowAccumulationLimit(m_flowAccumulationLimit);
    switch (m_drainageNetworkTypeLimit)
    {
        case OnlyMain:
            directionCalculator.useOnlyMainDrainageNetwork();
            break;
        case Amount:
            directionCalculator.useDrainageNetworkAmountLimit(m_drainageNetworkAmountLimit);
            break;
        case Percent:
            directionCalculator.useDrainageNetworkPercentLimit(m_drainageNetworkPercentLimit);
            break;
        default:
            throw std::runtime_error("The limit of the drainage networks has not been defined.");
    }
    directionCalculator.execute();

    std::shared_ptr<std::vector<std::shared_ptr<DrainageNetwork>>> drainageNetworks = directionCalculator.getDrainageNetworks();

    for (const std::shared_ptr<DrainageNetwork> &drainageNetwork : *drainageNetworks)
    {
        const size_t positionsSize = drainageNetwork->positions.size();

        std::vector<std::vector<double>> initialRaster(m_numberOfRows, std::vector<double>(m_numberOfCols));
        for (size_t row = 0; row < m_numberOfRows; ++row)
        {
            for (size_t col = 0; col < m_numberOfCols; ++col)
            {
                initialRaster[row][col] = m_raster->getData(row, col);
            }
        }

        const std::vector<std::vector<double>> pastSummation = donorsSummation(drainageNetwork->positions, true);

        for (size_t indexIteration = 1; indexIteration < m_numberOfIterations; ++indexIteration)
        {
            const std::vector<std::vector<double>> futureSummation = donorsSummation(drainageNetwork->positions, false);

            for (size_t pos = 1; pos < positionsSize; ++pos)
            {
                const PositionMatrix &position = drainageNetwork->positions[pos];

                const size_t row = position.row;
                const size_t col = position.col;

                // Para garantir que a topografia do ponto 0 nunca se altere aolongo das iterações
                // A topogrfia do ponto 0 é constante
                m_raster->setData(drainageNetwork->positions[0].row, drainageNetwork->positions[0].col, initialRaster[drainageNetwork->positions[0].row][drainageNetwork->positions[0].col]);

                size_t fdrRow = position.row;
                size_t fdrCol = position.col;

                double facValue = m_flowAccumulation->getData(row, col);

                moveToFlowDirection(m_flowDirection->getData(fdrRow, fdrCol), fdrRow, fdrCol, m_numberOfRows, m_numberOfCols);

                double bi = initialRaster[row][col] + (m_dimensionLessDepositionCoeficient / (m_dimensionLessPrecipitationRate * facValue)) * pastSummation[row][col];

                if (m_uplift != nullptr)
                {
                    bi += m_uplift->getData(row, col) * m_deltaTime;
                }

                const double f = (m_erodibility * std::pow(m_dimensionLessPrecipitationRate, m_concavityIndex) * std::pow(facValue * m_cellSize * m_cellSize, m_concavityIndex) * m_deltaTime) / m_cellSize;

                m_raster->setData(row, col, (bi - (m_dimensionLessDepositionCoeficient / (m_dimensionLessPrecipitationRate * facValue)) * futureSummation[row][col] + f * m_raster->getData(fdrRow, fdrCol)) / (1 + f));
            }
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
