#include "continental/landscapeevolutionmodel/service/UpliftAlgorithmService.h"
#include <continental/datamanagement/Raster.h>
#include <continental/datamanagement/RasterFile.h>
#include <QString>
#include <QDebug>
#include <memory>
#include <cmath>
#include <vector>

using namespace continental::datamanagement;

namespace continental {
namespace landscapeevolutionmodel {
namespace service {


UpliftAlgorithmService::UpliftAlgorithmService()
{

}


UpliftAlgorithmService::UpliftAlgorithmService(std::shared_ptr<datamanagement::Raster<double>> initialGrid,
                                               std::shared_ptr<datamanagement::Raster<double> > upliftRate,
                                               double timeStep,
                                               size_t numberOfTimeSteps) :
    m_initialGrid(initialGrid),
    m_upliftRate(upliftRate),
    m_timeStep(timeStep),
    m_numberOfTimeSteps(numberOfTimeSteps)
{

}

std::shared_ptr<datamanagement::Raster<double> > UpliftAlgorithmService::getInitialGrid() const
{
    return m_initialGrid;
}

void UpliftAlgorithmService::setInitialGrid(const std::shared_ptr<datamanagement::Raster<double> > &initialGrid)
{
    m_initialGrid = initialGrid;
}

double UpliftAlgorithmService::getTimeStep() const
{
    return m_timeStep;
}

void UpliftAlgorithmService::setTimeStep(double timeStep)
{
    m_timeStep = timeStep;
}

std::shared_ptr<datamanagement::Raster<double> > UpliftAlgorithmService::getUpliftRate() const
{
    return m_upliftRate;
}

void UpliftAlgorithmService::setUpliftRate(const std::shared_ptr<datamanagement::Raster<double> > &upliftRate)
{
    m_upliftRate = upliftRate;
}

size_t UpliftAlgorithmService::getNumberOfTimeSteps() const
{
    return m_numberOfTimeSteps;
}

void UpliftAlgorithmService::setNumberOfTimeSteps(const size_t &numberOfTimeSteps)
{
    m_numberOfTimeSteps = numberOfTimeSteps;
}

void UpliftAlgorithmService::executeUplift()
{

    for (size_t i = 0; i < m_initialGrid->getRows(); ++i)
    {
        for(size_t j = 0; j < m_initialGrid->getCols(); ++j)
        {
            m_initialGrid->setData(i, j, m_upliftRate->getData(i, j) * m_timeStep + m_initialGrid->getData(i, j));
        }
    }

}

std::shared_ptr<datamanagement::Raster<double>>  UpliftAlgorithmService::totalUplift()
{

    std::shared_ptr<datamanagement::Raster<double>> totalUpliftResult;

    for (size_t i = 0; i < m_initialGrid->getRows(); ++i)
    {
        for(size_t j = 0; j < m_initialGrid->getCols(); ++j)
        {
            totalUpliftResult->setData(i, j, m_upliftRate->getData(i, j) * m_timeStep * m_numberOfTimeSteps);
        }
    }

    return totalUpliftResult;
}


}
}
}
