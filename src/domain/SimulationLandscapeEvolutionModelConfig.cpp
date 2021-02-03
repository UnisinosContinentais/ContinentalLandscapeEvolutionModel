#include "continental/landscapeevolutionmodel/domain/SimulationLandscapeEvolutionModelConfig.h"
#include <QString>

namespace continental {
namespace landscapeevolutionmodel {
namespace domain {

unsigned short SimulationLandscapeEvolutionModelConfig::getVersion() const
{
    return m_version;
}

void SimulationLandscapeEvolutionModelConfig::setVersion(unsigned short version)
{
    m_version = version;
}

double SimulationLandscapeEvolutionModelConfig::getErodibility() const
{
    return m_erodibility;
}

void SimulationLandscapeEvolutionModelConfig::setErodibility(const double &erodibility)
{
    m_erodibility = erodibility;
}

double SimulationLandscapeEvolutionModelConfig::getDiffusion() const
{
    return m_diffusion;
}

void SimulationLandscapeEvolutionModelConfig::setDiffusion(const double &diffusion)
{
    m_diffusion = diffusion;
}

double SimulationLandscapeEvolutionModelConfig::getValueM() const
{
    return m_valueM;
}

void SimulationLandscapeEvolutionModelConfig::setValueM(const double &valueM)
{
    m_valueM = valueM;
}

double SimulationLandscapeEvolutionModelConfig::getValueN() const
{
    return m_valueN;
}

void SimulationLandscapeEvolutionModelConfig::setValueN(const double &valueN)
{
    m_valueN = valueN;
}

size_t SimulationLandscapeEvolutionModelConfig::getEastBoundaryFactor() const
{
    return m_eastBoundaryFactor;
}

void SimulationLandscapeEvolutionModelConfig::setEastBoundaryFactor(const size_t &eastBoundaryFactor)
{
    m_eastBoundaryFactor = eastBoundaryFactor;
}

size_t SimulationLandscapeEvolutionModelConfig::getWestBoundaryFactor() const
{
    return m_westBoundaryFactor;
}

void SimulationLandscapeEvolutionModelConfig::setWestBoundaryFactor(const size_t &westBoundaryFactor)
{
    m_westBoundaryFactor = westBoundaryFactor;
}

size_t SimulationLandscapeEvolutionModelConfig::getSouthBoundaryFactor() const
{
    return m_southBoundaryFactor;
}

void SimulationLandscapeEvolutionModelConfig::setSouthBoundaryFactor(const size_t &southBoundaryFactor)
{
    m_southBoundaryFactor = southBoundaryFactor;
}

size_t SimulationLandscapeEvolutionModelConfig::getNorthBoundaryFactor() const
{
    return m_northBoundaryFactor;
}

void SimulationLandscapeEvolutionModelConfig::setNorthBoundaryFactor(const size_t &northBoundaryFactor)
{
    m_northBoundaryFactor = northBoundaryFactor;
}

size_t SimulationLandscapeEvolutionModelConfig::getDrainagesLength() const
{
    return m_drainagesLength;
}

void SimulationLandscapeEvolutionModelConfig::setDrainagesLength(const size_t &drainagesLength)
{
    m_drainagesLength = drainagesLength;
}

}
}
}
