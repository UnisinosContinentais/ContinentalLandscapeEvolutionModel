#include "continental/landscapeevolutionmodel/domain/Direction.h"

namespace continental {
namespace landscapeevolutionmodel {
namespace domain {

Direction::Direction(size_t row, size_t col)
    : m_row(row)
    , m_col(col)
{
}

size_t Direction::getRow() const
{
    return m_row;
}

size_t Direction::getCol() const
{
    return m_col;
}

} // namespace domain
} // namespace landscapeevolutionmodel
} // namespace continental
