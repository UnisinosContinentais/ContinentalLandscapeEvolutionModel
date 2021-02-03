#ifndef CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_DOMAIN_POSITIONMATRIX_H
#define CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_DOMAIN_POSITIONMATRIX_H

#include "../export.h"

namespace continental {
namespace landscapeevolutionmodel {
namespace domain {

class CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_EXPORT_DECL PositionMatrix
{
public:
    /// Construtor.
    PositionMatrix() = default;

    /// Contrutor.
    /// @param row posição da linha na matriz.
    /// @param col posição da coluna na matriz.
    PositionMatrix(size_t row, size_t col);

    /// Atributo com o valor de linha da matriz.
    size_t row = 0;

    /// Atributo com o valor de coluna da matriz.
    size_t col = 0;
};

} // namespace domain
} // namespace landscapeevolutionmodel
} // namespace continental

#endif // CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_DOMAIN_POSITIONMATRIX_H
