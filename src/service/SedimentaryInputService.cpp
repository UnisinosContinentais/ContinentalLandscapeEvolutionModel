#include "continental/landscapeevolutionmodel/service/SedimentaryInputService.h"
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

SedimentaryInputService::SedimentaryInputService() :
    m_sedimentaryInputs(std::make_shared<std::vector<std::shared_ptr<domain::SedimentaryInputContent>>>())
{

}

void SedimentaryInputService::setOnlyErosionDepositionGrid(const std::shared_ptr<datamanagement::Raster<double>> & onlyErosionDepositionGrid)
{
    m_onlyErosionDepositionGrid = onlyErosionDepositionGrid;
}

void SedimentaryInputService::setFlowDirection(const std::shared_ptr<Raster<short>> & flowDirection)
{
	m_flowDirection = flowDirection;
}

void SedimentaryInputService::setFlowAccumulation(const std::shared_ptr<Raster<int>> & flowAccumulation)
{
	m_flowAccumulation = flowAccumulation;
}

void SedimentaryInputService::useOnlyMainDrainageNetwork()
{
    m_drainageNetworkTypeLimit = OnlyMain;
}

void SedimentaryInputService::useDrainageNetworkAmountLimit(size_t amountLimit)
{
    m_drainageNetworkTypeLimit = Amount;
    m_drainageNetworkAmountLimit = amountLimit;
}

void SedimentaryInputService::useDrainageNetworkPercentLimit(double percentLimit)
{
    m_drainageNetworkTypeLimit = Percent;
    m_drainageNetworkPercentLimit = percentLimit;
}

int SedimentaryInputService::getFlowAccumulationLimit() const
{
    return m_flowAccumulationLimit;
}

void SedimentaryInputService::setFlowAccumulationLimit(int flowAccumulationLimit)
{
    m_flowAccumulationLimit = flowAccumulationLimit;
}

const std::shared_ptr<std::vector<std::shared_ptr<domain::SedimentaryInputContent>>> & SedimentaryInputService::sedimentaryInputs() const
{
    return m_sedimentaryInputs;
}

void SedimentaryInputService::execute()
{
    qDebug("\n\n *** SedimentaryInputService::execute() *** \n\n");

    m_numberOfCols = m_flowDirection->getCols();
    m_numberOfRows = m_flowDirection->getRows();
    m_cellSize = m_flowDirection->getCellSize();

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

    const std::shared_ptr<std::vector<std::shared_ptr<DrainageNetwork>>> & drainageNetworks = directionCalculator.getDrainageNetworks();

    qDebug("\n\n *** Lista de Exutórios *** \n\n");

    for (const std::shared_ptr<DrainageNetwork> &drainageNetwork : *drainageNetworks)
    {
        bool isFirst = true;

        double valueSedimentaryInput = 0.0;
        size_t posRowSedimentaryInput = 0;
        size_t posColSedimentaryInput = 0;

        for (PositionMatrix &position : drainageNetwork->positions)
        {
            const size_t & row = position.row;
            const size_t & col = position.col;

            if (isFirst)
            {
                // Definir o exutório...
                posRowSedimentaryInput = row;
                posColSedimentaryInput = col;

                // Verificar nível do mar aqui...


                isFirst = false;
            }

            valueSedimentaryInput += m_onlyErosionDepositionGrid->getData(row, col);
        }

        valueSedimentaryInput = valueSedimentaryInput * (m_cellSize*m_cellSize);

        auto sedimentaryInputContent = std::make_shared<domain::SedimentaryInputContent>();

        sedimentaryInputContent->setPositionI(posRowSedimentaryInput);
        sedimentaryInputContent->setPositionJ(posColSedimentaryInput);
        sedimentaryInputContent->setSedimentaryInputContent(valueSedimentaryInput);

        m_sedimentaryInputs->push_back(sedimentaryInputContent);
    }
}

}
}
}
