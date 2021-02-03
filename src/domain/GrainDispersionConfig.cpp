#include "continental/landscapeevolutionmodel/domain/GrainDispersionConfig.h"
#include <QString>

namespace continental {
namespace landscapeevolutionmodel {
namespace domain {

double GrainDispersionConfig::getDischargeKParameter() const
{
    return m_channelDepthCParameter;
}

void GrainDispersionConfig::setDischargeKParameter(double dischargeKParameter)
{
    m_dischargeKParameter = dischargeKParameter;
}

double GrainDispersionConfig::getDischargeEParameter() const
{
    return m_dischargeEParameter;
}

void GrainDispersionConfig::setDischargeEParameter(double dischargeEParameter)
{
    m_dischargeEParameter = dischargeEParameter;
}

double GrainDispersionConfig::getChannelDepthCParameter() const
{
    return m_channelDepthCParameter;
}

void GrainDispersionConfig::setChannelDepthCParameter(double channelDepthCParameter)
{
    m_channelDepthCParameter = channelDepthCParameter;
}

double GrainDispersionConfig::getChannelDepthFParameter() const
{
    return m_channelDepthFParameter;
}

void GrainDispersionConfig::setChannelDepthFParameter(double channelDepthFParameter)
{
    m_channelDepthFParameter = channelDepthFParameter;
}

double GrainDispersionConfig::getGrainSizeWaterDensity() const
{
    return m_grainSizeWaterDensity;
}

void GrainDispersionConfig::setGrainSizeWaterDensity(double grainSizeWaterDensity)
{
    m_grainSizeWaterDensity = grainSizeWaterDensity;
}

double GrainDispersionConfig::getGrainSizeSedimentDensity() const
{
    return m_grainSizeSedimentDensity;
}

void GrainDispersionConfig::setGrainSizeSedimentDensity(double grainSizeSedimentDensity)
{
    m_grainSizeSedimentDensity = grainSizeSedimentDensity;
}

double GrainDispersionConfig::getGrainSizeShieldsNumber() const
{
    return m_grainSizeShieldsNumber;
}

void GrainDispersionConfig::setGrainSizeShieldsNumber(double grainSizeShieldsNumber)
{
    m_grainSizeShieldsNumber = grainSizeShieldsNumber;
}

}
}
}
