#include "continental/landscapeevolutionmodel/domain/StreamDefinitionConfig.h"
#include <QString>

namespace continental {
namespace landscapeevolutionmodel {
namespace domain {

double StreamDefinitionConfig::getThresoldValue() const
{
    return m_thresoldValue;
}

void StreamDefinitionConfig::setThresoldValue(double ThresoldValue)
{
    m_thresoldValue = ThresoldValue;
}

StreamDefinitionThresholdType StreamDefinitionConfig::getThresoldType() const
{
    return m_thresoldType;
}

QString StreamDefinitionConfig::getThresoldTypeName() const
{
    if (m_thresoldType == StreamDefinitionThresholdType::Area)
    {
        return "Área aproximada";
    }
    else if (m_thresoldType == StreamDefinitionThresholdType::NumberOfCells)
    {
        return "Número de Células";
    }
    else if (m_thresoldType == StreamDefinitionThresholdType::PercentualOfMaximumCells)
    {
        return "Percentual do número de células";
    }
    else
        return "";
}

void StreamDefinitionConfig::setThresoldType(QString thresoldTypeId)
{
    if (thresoldTypeId == "Area")
    {
        m_thresoldType  = StreamDefinitionThresholdType::Area;
    }
    else if (thresoldTypeId == "NumberOfCells")
    {
        m_thresoldType  = StreamDefinitionThresholdType::NumberOfCells;
    }
    else if (thresoldTypeId == "PercentualOfMaximumCells")
    {
        m_thresoldType  = StreamDefinitionThresholdType::PercentualOfMaximumCells;
    }
    else
        m_thresoldType  = StreamDefinitionThresholdType::NumberOfCells;
}

} // namespace domain
} // namespace landscapeevolutionmodel
} // namespace continental
