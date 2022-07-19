#include "continental/landscapeevolutionmodel/domain/SedimentaryInputContent.h"

namespace continental {
namespace landscapeevolutionmodel {
namespace domain {

SedimentaryInputContent::SedimentaryInputContent()
{
}

const unsigned long long & SedimentaryInputContent::getPositionI() const
{
    return m_positionI;
}

void SedimentaryInputContent::setPositionI(const unsigned long long &positionI)
{
    m_positionI = positionI;
}

const unsigned long long & SedimentaryInputContent::getPositionJ() const
{
    return m_positionJ;
}

void SedimentaryInputContent::setPositionJ(const unsigned long long &positionJ)
{
    m_positionJ = positionJ;
}

const double & SedimentaryInputContent::getSedimentaryInputContent() const
{
    return m_sedimentaryInputContent;
}

void SedimentaryInputContent::setSedimentaryInputContent(const double &sedimentaryInputContent)
{
    m_sedimentaryInputContent = sedimentaryInputContent;
}

}
}
}

