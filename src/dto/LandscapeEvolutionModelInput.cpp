#include "continental/landscapeevolutionmodel/dto/LandscapeEvolutionModelInput.h"
#include "continental/landscapeevolutionmodel/domain/SinkDestroyConfig.h"
#include "continental/landscapeevolutionmodel/domain/StreamDefinitionConfig.h"

using namespace continental::landscapeevolutionmodel::domain;

namespace continental {
namespace landscapeevolutionmodel {
namespace dto {

LandscapeEvolutionModelInput::LandscapeEvolutionModelInput()
{

}

std::shared_ptr<SinkDestroyConfig> LandscapeEvolutionModelInput::getSinkDestroyConfig() const
{
    return m_sinkDestroyConfig;
}

void LandscapeEvolutionModelInput::setSinkDestroyConfig(const std::shared_ptr<SinkDestroyConfig> &sinkDestroyConfig)
{
    m_sinkDestroyConfig = sinkDestroyConfig;
}

std::shared_ptr<StreamDefinitionConfig> LandscapeEvolutionModelInput::getStreamDefinitionConfig() const
{
    return m_streamDefinitionConfig;
}

void LandscapeEvolutionModelInput::setStreamDefinitionConfig(const std::shared_ptr<StreamDefinitionConfig> &streamDefinitionConfig)
{
    m_streamDefinitionConfig = streamDefinitionConfig;
}

std::shared_ptr<SimulationLandscapeEvolutionModelConfig> LandscapeEvolutionModelInput::getSimulationLandscapeEvolutionModelConfig() const
{
    return m_simulationLandscapeEvolutionModelConfig;
}

void LandscapeEvolutionModelInput::setSimulationLandscapeEvolutionModelConfig(const std::shared_ptr<SimulationLandscapeEvolutionModelConfig> &simulationLandscapeEvolutionModelConfig)
{
    m_simulationLandscapeEvolutionModelConfig = simulationLandscapeEvolutionModelConfig;
}

void LandscapeEvolutionModelInput::setSimulateUntilTime(const size_t simulateUntilTime)
{
    m_simulateUntilTime = simulateUntilTime;
}

std::shared_ptr<continental::landscapeevolutionmodel::domain::GrainDispersionConfig> LandscapeEvolutionModelInput::getGrainDispersionConfig() const
{
    return m_grainDispersionConfig;
}

void LandscapeEvolutionModelInput::setGrainDispersionConfig(const std::shared_ptr<continental::landscapeevolutionmodel::domain::GrainDispersionConfig> &grainDispersionConfig)
{
    m_grainDispersionConfig = grainDispersionConfig;
}

bool LandscapeEvolutionModelInput::getEnableSurfaceLog() const
{
    return m_enableSurfaceLog;
}

void LandscapeEvolutionModelInput::setEnableSurfaceLog(bool enableSurfaceLog)
{
    m_enableSurfaceLog = enableSurfaceLog;
}

QString LandscapeEvolutionModelInput::getPathSurfaceLog() const
{
    return m_pathSurfaceLog;
}

void LandscapeEvolutionModelInput::setPathSurfaceLog(const QString &pathSurfaceLog)
{
    m_pathSurfaceLog = pathSurfaceLog;
}

double LandscapeEvolutionModelInput::getAge() const
{
    return m_age;
}

void LandscapeEvolutionModelInput::setAge(double age)
{
    m_age = age;
}

std::shared_ptr<datamanagement::Raster<double> > LandscapeEvolutionModelInput::getUpliftRate() const
{
    return m_upliftRate;
}

void LandscapeEvolutionModelInput::setUpliftRate(const std::shared_ptr<datamanagement::Raster<double> > &upliftRate)
{
    m_upliftRate = upliftRate;
}

size_t LandscapeEvolutionModelInput::getSimulateUntilTime() const
{
    return m_simulateUntilTime;
}



} //dto
} //landscapeevolutionmodel
} //continental
