#include "continental/landscapeevolutionmodel/domain/SinkDestroyConfig.h"
#include <QString>

namespace continental {
namespace landscapeevolutionmodel {
namespace domain {

size_t SinkDestroyConfig::getMaxOpenList() const
{
    return m_maxOpenList;
}

void SinkDestroyConfig::setMaxOpenList(size_t value)
{
    m_maxOpenList = value;
}

size_t SinkDestroyConfig::getMaxClosedList() const
{
    return m_maxClosedList;
}

void SinkDestroyConfig::setMaxClosedList(size_t value)
{
    m_maxClosedList = value;
}

float SinkDestroyConfig::getCostFunctionWeight() const
{
    return m_costFunctionWeight;
}

void SinkDestroyConfig::setCostFunctionWeight(float value)
{
    m_costFunctionWeight = value;
}

HeuristicSinkRemovalProcessingMode SinkDestroyConfig::getProcessingAlgorithm() const
{
    return m_processingAlgorithm;
}

QString SinkDestroyConfig::getProcessingAlgorithmName() const
{
    if (m_processingAlgorithm == HeuristicSinkRemovalProcessingMode::HS)
    {
        return "HS";
    }
    else if (m_processingAlgorithm == HeuristicSinkRemovalProcessingMode::MHS)
    {
        return "MHS";
    }
    else if (m_processingAlgorithm == HeuristicSinkRemovalProcessingMode::PFS)
    {
        return "PFS";
    }
    else
        return "";
}

void SinkDestroyConfig::setProcessingAlgorithm(QString value)
{
    if (value == "HS")
    {
        m_processingAlgorithm =  HeuristicSinkRemovalProcessingMode::HS;
    }
    else if (value == "MHS")
    {
        m_processingAlgorithm = HeuristicSinkRemovalProcessingMode::MHS;
    }
    else if (value == "PFS")
    {
        m_processingAlgorithm = HeuristicSinkRemovalProcessingMode::PFS;
    }
    else
    {
        m_processingAlgorithm = HeuristicSinkRemovalProcessingMode::MHS;
    }

}

unsigned short SinkDestroyConfig::getVersion() const
{
    return m_version;
}

void SinkDestroyConfig::setVersion(unsigned short version)
{
    m_version = version;
}

}
}
}
