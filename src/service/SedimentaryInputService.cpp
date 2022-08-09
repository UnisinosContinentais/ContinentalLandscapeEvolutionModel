#include "continental/landscapeevolutionmodel/service/SedimentaryInputService.h"
#include "continental/landscapeevolutionmodel/dto/LandscapeEvolutionModelInput.h"
#include <continental/datamanagement/Raster.h>
#include <continental/datamanagement/RasterFile.h>
#include "continental/hydrotools/service/HeuristicSinkRemovalUtil.h"
#include <continental/hydrotools/service/HeuristicSinkRemoval.h>
#include <continental/hydrotools/service/HeuristicSinkRemovalUtil.h>
#include <continental/hydrotools/service/FlowDirection.h>
#include <continental/hydrotools/service/CellWatershed.h>
#include "continental/landscapeevolutionmodel/domain/EnumDirection.h"
#include "continental/landscapeevolutionmodel/service/HydroToolsAlgorithmService.h"
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

void SedimentaryInputService::setWaterShed(const std::shared_ptr<Raster<short>> & waterShed)
{
    m_waterShed = waterShed;
}

void SedimentaryInputService::setCellExhilarating(const std::shared_ptr<std::vector<std::shared_ptr<hydrotools::service::CellWatershed>>> & cellExhilarating)
{
    m_cellExhilarating = cellExhilarating;
}

const std::shared_ptr<std::vector<std::shared_ptr<domain::SedimentaryInputContent>>> & SedimentaryInputService::sedimentaryInputs() const
{
    return m_sedimentaryInputs;
}

void SedimentaryInputService::setSimulateUntilTime(const double simulateUntilTime)
{
    m_simulateUntilTime = simulateUntilTime;
}

void SedimentaryInputService::execute()
{
    qDebug("\n\n *** SedimentaryInputService::execute() *** \n\n");

    m_numberOfCols = m_waterShed->getCols();
    m_numberOfRows = m_waterShed->getRows();
    m_cellSize = m_waterShed->getCellSize();

    qDebug("\n\n *** Lista de Exutórios *** \n\n");

    for (const std::shared_ptr<hydrotools::service::CellWatershed> &cellWatershed : *m_cellExhilarating)
    {
        double valueSedimentaryInput = 0.0;
        const size_t & posRowSedimentaryInput = cellWatershed->y;
        const size_t & posColSedimentaryInput = cellWatershed->x;

        const short & watershedId = cellWatershed->getAttribute();

        for (size_t i = 0; i < m_numberOfRows; ++i)
        {
            for (size_t j = 0; j < m_numberOfCols; ++j)
            {
                if (m_waterShed->getData(i, j) == watershedId)
                {
                    valueSedimentaryInput += m_onlyErosionDepositionGrid->getData(i, j);
                }
            }
        }

        valueSedimentaryInput = valueSedimentaryInput * (m_cellSize * m_cellSize);

        //valueSedimentaryInput = valueSedimentaryInput / (m_simulateUntilTime * 1000000.0 );

        valueSedimentaryInput = valueSedimentaryInput / (m_simulateUntilTime);

        valueSedimentaryInput = valueSedimentaryInput / (1000000.0); // transformação Mm^3/ano



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
