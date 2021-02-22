#ifndef CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_SERVICE_DIRECTIONCALCULATORSERVICE_H
#define CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_SERVICE_DIRECTIONCALCULATORSERVICE_H

#include "continental/landscapeevolutionmodel/export.h"
#include "continental/landscapeevolutionmodel/domain/Direction.h"
#include "continental/landscapeevolutionmodel/domain/DrainageNetwork.h"
#include "continental/landscapeevolutionmodel/domain/PositionMatrix.h"
#include <continental/datamanagement/Raster.h>
#include <memory>

namespace continental {
namespace landscapeevolutionmodel {
namespace service {

class CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_EXPORT_DECL DirectionCalculatorService
{
public:
    static const short DIRECTION_RIGHT = 1;
    static const short DIRECTION_DOWN_RIGHT = 2;
    static const short DIRECTION_DOWN = 4;
    static const short DIRECTION_DOWN_LEFT = 8;
    static const short DIRECTION_LEFT = 16;
    static const short DIRECTION_UP_LEFT = 32;
    static const short DIRECTION_UP = 64;
    static const short DIRECTION_UP_RIGHT = 128;

    DirectionCalculatorService(std::shared_ptr<datamanagement::Raster<short>> flowDirection, std::shared_ptr<datamanagement::Raster<int>> flowAccumulation);

    void setFlowDirection(const std::shared_ptr<datamanagement::Raster<short>> flowDirection);
    std::shared_ptr<datamanagement::Raster<short>> getFlowDirection() const;

    std::shared_ptr<datamanagement::Raster<int>> getFlowAccumulation() const;
    void setFlowAccumulation(const std::shared_ptr<datamanagement::Raster<int>> flowAccumulation);

    bool getProcessMapsPositionsAndBranches() const;
    void setProcessMapsPositionsAndBranches(bool processMapsPositionsAndBranches);

    double getFacLimit() const;
    void setFacLimit(double facLimit);

    std::shared_ptr<std::vector<std::shared_ptr<domain::DrainageNetwork>>> getDrainageNetworks() const;

    void execute(bool onlyMainDrainageNetwork = true);
private:
    const int LimitMapPositions = 1;

    std::shared_ptr<datamanagement::Raster<short>> m_flowDirection;
    std::shared_ptr<datamanagement::Raster<int>> m_flowAccumulation;

    bool m_processMapsPositionsAndBranches = false;
    double m_facLimit = -1;
    size_t m_rows = 0;
    size_t m_cols = 0;
	
    double m_noDataValue = 0.0f;
    std::shared_ptr<std::vector<std::shared_ptr<domain::DrainageNetwork>>> m_drainageNetworks;

    void makeTree(domain::DrainageNetwork &drainageNetwork, datamanagement::Raster<int> &flowAccumulation, size_t exuterRow, size_t exuterColumn);

    size_t makeTree(
            domain::Direction &direction,
            datamanagement::Raster<int> &flowAccumulation,
            size_t numberOfCellsOfIndentified = 0
        );

    void makeTreeDonors(domain::DrainageNetwork &drainageNetwork);

    std::pair<short, size_t> makeTreeDonors(domain::DrainageNetwork &drainageNetwork, const domain::Direction &direction, size_t position, short numberOfTree);
};

} // namespace service
} // namespace landscapeevolutionmodel
} // namespace continental

#endif // CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_SERVICE_DIRECTIONCALCULATORSERVICE_H
