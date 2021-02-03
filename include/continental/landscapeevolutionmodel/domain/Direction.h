/*
* Developed by UNISINOS - Projeto Continentais and Petrobras
* author: Luiz Felipe Bertoldi de Oliveira
* email: lbertoldio@unisinos.br
* date: October, 2020
*/
#ifndef CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_DOMAIN_DIRECTION_H
#define CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_DOMAIN_DIRECTION_H

#include <vector>
#include <memory>

namespace continental {
namespace landscapeevolutionmodel {
namespace domain {

class Direction
{
public:
    Direction(size_t row, size_t col);
    size_t getRow() const;
    size_t getCol() const;

    std::vector<Direction> donors;
private:
    size_t m_row = 0;
    size_t m_col = 0;
};

} // namespace domain
} // namespace landscapeevolutionmodel
} // namespace continental

#endif // CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_DOMAIN_DIRECTION_H
