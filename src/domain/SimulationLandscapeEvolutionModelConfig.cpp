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

double SimulationLandscapeEvolutionModelConfig::getDiffusivity() const
{
    return m_diffusivity;
}

void SimulationLandscapeEvolutionModelConfig::setDiffusivity(const double &diffusivity)
{
    m_diffusivity = diffusivity;
}

double SimulationLandscapeEvolutionModelConfig::getConcavityIndex() const
{
    return m_concavityIndex;
}

void SimulationLandscapeEvolutionModelConfig::setConcavityIndex(const double &concavityIndex)
{
    m_concavityIndex = concavityIndex;
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

double SimulationLandscapeEvolutionModelConfig::getDimensionLessPrecipitationRate() const
{
    return m_dimensionLessPrecipitationRate;
}

void SimulationLandscapeEvolutionModelConfig::setDimensionLessPrecipitationRate(double dimensionLessPrecipitationRate)
{
    m_dimensionLessPrecipitationRate = dimensionLessPrecipitationRate;
}

double SimulationLandscapeEvolutionModelConfig::getDimensionLessDepositionCoeficient() const
{
    return m_dimensionLessDepositionCoeficient;
}

void SimulationLandscapeEvolutionModelConfig::setDimensionLessDepositionCoeficient(double dimensionLessDepositionCoeficient)
{
    m_dimensionLessDepositionCoeficient = dimensionLessDepositionCoeficient;
}

void SimulationLandscapeEvolutionModelConfig::useOnlyMainDrainageNetwork()
{
    m_drainageNetworkTypeLimit = OnlyMain;
}

void SimulationLandscapeEvolutionModelConfig::useDrainageNetworkAmountLimit(size_t amountLimit)
{
    m_drainageNetworkTypeLimit = Amount;
    m_drainageNetworkAmountLimit = amountLimit;
}

void SimulationLandscapeEvolutionModelConfig::useDrainageNetworkPercentLimit(double percentLimit)
{
    m_drainageNetworkTypeLimit = Percent;
    m_drainageNetworkPercentLimit = percentLimit;
}

domain::EnumDrainageNetworkLimit SimulationLandscapeEvolutionModelConfig::getDrainageNetworkTypeLimit() const
{
    return m_drainageNetworkTypeLimit;
}

size_t SimulationLandscapeEvolutionModelConfig::getDrainageNetworkAmountLimit() const
{
    return m_drainageNetworkAmountLimit;
}

double SimulationLandscapeEvolutionModelConfig::getDrainageNetworkPercentLimit() const
{
    return m_drainageNetworkPercentLimit;
}


} // namespace domain
} // namespace landscapeevolutionmodel
} // namespace continental
