#ifndef CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_SERVICE_DIRECTIONCALCULATORSERVICE_H
#define CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_SERVICE_DIRECTIONCALCULATORSERVICE_H

#include "continental/landscapeevolutionmodel/export.h"
#include "continental/landscapeevolutionmodel/domain/Direction.h"
#include "continental/landscapeevolutionmodel/domain/DrainageNetwork.h"
#include "continental/landscapeevolutionmodel/domain/EnumDrainageNetworkLimit.h"
#include "continental/landscapeevolutionmodel/domain/PositionMatrix.h"
#include <continental/datamanagement/Raster.h>
#include <memory>

namespace continental {
namespace landscapeevolutionmodel {
namespace service {

class CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_EXPORT_DECL DirectionCalculatorService
{
public:
    DirectionCalculatorService(std::shared_ptr<datamanagement::Raster<short>> flowDirection, std::shared_ptr<datamanagement::Raster<int>> flowAccumulation);

    void setFlowDirection(const std::shared_ptr<datamanagement::Raster<short>> flowDirection);
    std::shared_ptr<datamanagement::Raster<short>> getFlowDirection() const;

    std::shared_ptr<datamanagement::Raster<int>> getFlowAccumulation() const;
    void setFlowAccumulation(const std::shared_ptr<datamanagement::Raster<int>> flowAccumulation);

    bool getProcessMapsPositionsAndBranches() const;
    void setProcessMapsPositionsAndBranches(bool processMapsPositionsAndBranches);

    int getFlowAccumulationLimit() const;
    void setFlowAccumulationLimit(int flowAccumulationLimit);

    std::shared_ptr<std::vector<std::shared_ptr<domain::DrainageNetwork>>> getDrainageNetworks() const;

    void execute();

    void useOnlyMainDrainageNetwork();

    void useDrainageNetworkAmountLimit(size_t amountLimit);

    void useDrainageNetworkPercentLimit(double percentLimit);
private:
    const int LimitMapPositions = std::numeric_limits<int>::max();

    std::shared_ptr<datamanagement::Raster<short>> m_flowDirection;
    std::shared_ptr<datamanagement::Raster<int>> m_flowAccumulation;

    bool m_processMapsPositionsAndBranches = false;
    int m_flowAccumulationLimit = -1;
    size_t m_rows = 0;
    size_t m_cols = 0;
	float m_noDataValue = 0.0f;
    std::shared_ptr<std::vector<std::shared_ptr<domain::DrainageNetwork>>> m_drainageNetworks;

    domain::EnumDrainageNetworkLimit m_drainageNetworkTypeLimit = domain::EnumDrainageNetworkLimit::Undefined;
    size_t m_drainageNetworkAmountLimit = 0;
    double m_drainageNetworkPercentLimit = 0.0;

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
