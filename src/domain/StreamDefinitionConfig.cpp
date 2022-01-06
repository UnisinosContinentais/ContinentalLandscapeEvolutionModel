#include "continental/landscapeevolutionmodel/domain/StreamDefinitionConfig.h"
#include <QString>

namespace continental {
namespace landscapeevolutionmodel {
namespace domain {

double StreamDefinitionConfig::getThresholdValue() const
{
    return m_thresholdValue;
}

void StreamDefinitionConfig::setThresholdValue(double ThresholdValue)
{
    m_thresholdValue = ThresholdValue;
}

StreamDefinitionThresholdType StreamDefinitionConfig::getThresholdType() const
{
    return m_thresholdType;
}

QString StreamDefinitionConfig::getThresholdTypeName() const
{
    if (m_thresholdType == StreamDefinitionThresholdType::Area)
    {
        return "Área aproximada";
    }
    else if (m_thresholdType == StreamDefinitionThresholdType::NumberOfCells)
    {
        return "Número de Células";
    }
    else if (m_thresholdType == StreamDefinitionThresholdType::PercentualOfMaximumCells)
    {
        return "Percentual do número de células";
    }
    else
        return "";
}

void StreamDefinitionConfig::setThresholdType(StreamDefinitionThresholdType thresholdType)
{
    m_thresholdType  = thresholdType;
}

} // namespace domain
} // namespace landscapeevolutionmodel
} // namespace continental
