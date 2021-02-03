#ifndef CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_DOMAIN_DRAINAGENETWORK_H
#define CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_DOMAIN_DRAINAGENETWORK_H

#include "continental/landscapeevolutionmodel/export.h"
#include "continental/landscapeevolutionmodel/domain/Direction.h"
#include "continental/landscapeevolutionmodel/domain/PositionMatrix.h"
#include <continental/datamanagement/Raster.h>

namespace continental {
namespace landscapeevolutionmodel {
namespace domain {

class CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_EXPORT_DECL DrainageNetwork
{
public:
    std::shared_ptr<datamanagement::Raster<short>> mapPositions;
    std::shared_ptr<datamanagement::Raster<short>> mapBranches;
    std::shared_ptr<domain::Direction> mainDirection;
    size_t numberOfCellsOfIndentified = 0;
    std::vector<domain::PositionMatrix> positions;
};

} // namespace domain
} // namespace landscapeevolutionmodel
} // namespace continental

#endif // CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_DOMAIN_DRAINAGENETWORK_H
