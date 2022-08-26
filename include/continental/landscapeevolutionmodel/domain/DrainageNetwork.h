#ifndef CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_DOMAIN_DRAINAGENETWORK_H
#define CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_DOMAIN_DRAINAGENETWORK_H

#include "continental/landscapeevolutionmodel/export.h"
#include "continental/landscapeevolutionmodel/domain/Direction.h"
#include "continental/landscapeevolutionmodel/domain/PositionMatrix.h"
#include <continental/datamanagement/Raster.h>

namespace continental {
namespace landscapeevolutionmodel {
namespace domain {

///
/// \brief The DrainageNetwork class Classe responsável pela rede de drenagem
///
class CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_EXPORT_DECL DrainageNetwork
{
public:

    /// mapPositions Variável map das posições
    std::shared_ptr<datamanagement::Raster<short>> mapPositions;

    /// mapBranches Variável map dos ramos
    std::shared_ptr<datamanagement::Raster<short>> mapBranches;

    /// mainDirection Variável referente a direção principal
    std::shared_ptr<domain::Direction> mainDirection;

    /// numberOfCellsOfIndentified Variável referente ao número de celulas
    /// identificadas
    size_t numberOfCellsOfIndentified = 0;

    /// positions Variável responsável pela posição
    std::vector<domain::PositionMatrix> positions;

};

} // namespace domain
} // namespace landscapeevolutionmodel
} // namespace continental

#endif // CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_DOMAIN_DRAINAGENETWORK_H
