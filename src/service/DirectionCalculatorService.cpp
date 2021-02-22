#include "continental/landscapeevolutionmodel/constant/LandscapeEvolutionModelConstant.h"
#include "continental/landscapeevolutionmodel/service/DirectionCalculatorService.h"
#include "continental/landscapeevolutionmodel/domain/Direction.h"
#include "continental/landscapeevolutionmodel/domain/EnumDirection.h"
#include "continental/landscapeevolutionmodel/domain/DrainageNetwork.h"
#include <iostream>

using namespace continental::datamanagement;
using namespace continental::landscapeevolutionmodel::constant;
using namespace continental::landscapeevolutionmodel::domain;

namespace continental {
namespace landscapeevolutionmodel {
namespace service {

DirectionCalculatorService::DirectionCalculatorService(std::shared_ptr<Raster<short>> flowDirection, std::shared_ptr<Raster<int>> flowAccumulation)
    : m_flowDirection(flowDirection)
    , m_flowAccumulation(flowAccumulation)
    , m_drainageNetworks(std::make_shared<std::vector<std::shared_ptr<domain::DrainageNetwork>>>())
{

}

void DirectionCalculatorService::setFlowDirection(const std::shared_ptr<Raster<short>> flowDirection)
{
    m_flowDirection = flowDirection;
}

std::shared_ptr<Raster<short>> DirectionCalculatorService::getFlowDirection() const
{
    return m_flowDirection;
}

std::shared_ptr<Raster<int>> DirectionCalculatorService::getFlowAccumulation() const
{
    return m_flowAccumulation;
}

void DirectionCalculatorService::setFlowAccumulation(const std::shared_ptr<Raster<int>> flowAccumulation)
{
    m_flowAccumulation = flowAccumulation;
}

void DirectionCalculatorService::makeTree(DrainageNetwork &drainageNetwork, Raster<int> &flowAccumulation, size_t exuterRow, size_t exuterColumn)
{
    drainageNetwork.mainDirection = std::make_shared<Direction>(exuterRow, exuterColumn);


    drainageNetwork.numberOfCellsOfIndentified = makeTree(*drainageNetwork.mainDirection, flowAccumulation, 0);
}

void DirectionCalculatorService::execute(bool onlyMainDrainageNetwork)
{
    Raster<int> flowAccumulation = *m_flowAccumulation;
    m_rows = flowAccumulation.getRows();
    m_cols = flowAccumulation.getCols();
	m_noDataValue = flowAccumulation.getNoDataValue();

    int limitDrainageNetwork = onlyMainDrainageNetwork ? 1 : LimitMapPositions;

    for (int indexDrainageNetwork = 0; indexDrainageNetwork < limitDrainageNetwork; ++indexDrainageNetwork)
    {
        size_t exuterRow = 0;
        size_t exuterColumn = 0;
		float exuterValue = 0.0f;
        bool found = false;

        for (size_t row = 0; row < m_rows; ++row)
        {
            for (size_t col = 0; col < m_cols; ++col)
            {
                double value = flowAccumulation.getData(row, col);

                if (exuterValue < value && value != m_noDataValue)
                {
                    exuterRow = row;
                    exuterColumn = col;
                    exuterValue = value;
                    found = true;
                }
            }
        }

		
        if (!found)
        {
            break;
        }

        auto drainageNetwork = std::make_shared<DrainageNetwork>();
        m_drainageNetworks->push_back(drainageNetwork);

        makeTree(*drainageNetwork, flowAccumulation, exuterRow, exuterColumn);
        makeTreeDonors(*drainageNetwork);
    }
}

float DirectionCalculatorService::getFlowAccumulationLimit() const
{
    return m_flowAccumulationLimit;
}

void DirectionCalculatorService::setFlowAccumulationLimit(float flowAccumulationLimit)
{
    m_flowAccumulationLimit = flowAccumulationLimit;
}

bool DirectionCalculatorService::getProcessMapsPositionsAndBranches() const
{
    return m_processMapsPositionsAndBranches;
}

void DirectionCalculatorService::setProcessMapsPositionsAndBranches(bool processMapsPositionsAndBranches)
{
    m_processMapsPositionsAndBranches = processMapsPositionsAndBranches;
}

std::shared_ptr<std::vector<std::shared_ptr<DrainageNetwork>>> DirectionCalculatorService::getDrainageNetworks() const
{
    return m_drainageNetworks;
}

size_t DirectionCalculatorService::makeTree(
        Direction &direction,
        Raster<int> &flowAccumulation,
        size_t numberOfCellsOfIndentified
    )
{
    const size_t row = direction.getRow();
    const size_t col = direction.getCol();
	flowAccumulation.setData(row, col, m_noDataValue);
	
    ++numberOfCellsOfIndentified;

    // RIGHT
    if (col - 1 >= 0 && m_flowDirection->getData(row, col - 1) == EnumDirection::Right && flowAccumulation.getData(row, col - 1) > m_flowAccumulationLimit && !qFuzzyCompare(m_noDataValue, flowAccumulation.getData(row, col - 1)))
    {
        direction.donors.emplace_back(Direction(row, col - 1));
        Direction &directionRight = direction.donors.back();
        numberOfCellsOfIndentified = makeTree(directionRight, flowAccumulation, numberOfCellsOfIndentified);
    }

    // DOWN and RIGHT
    if (row - 1 >= 0 && col - 1 >= 0 && m_flowDirection->getData(row - 1, col - 1) == EnumDirection::DownRight && flowAccumulation.getData(row - 1, col - 1) > m_flowAccumulationLimit && !qFuzzyCompare(m_noDataValue, flowAccumulation.getData(row - 1, col - 1)))
    {
        direction.donors.emplace_back(Direction(row - 1, col - 1));
        Direction &directionDownRight = direction.donors.back();
        numberOfCellsOfIndentified = makeTree(directionDownRight, flowAccumulation, numberOfCellsOfIndentified);
    }

    // DOWN
    if (row - 1 >= 0 && m_flowDirection->getData(row - 1, col) == EnumDirection::Down && flowAccumulation.getData(row - 1, col) > m_flowAccumulationLimit && !qFuzzyCompare(m_noDataValue, flowAccumulation.getData(row - 1, col)))
    {
        direction.donors.emplace_back(Direction(row - 1, col));
        Direction &directionDown = direction.donors.back();
        numberOfCellsOfIndentified = makeTree(directionDown, flowAccumulation, numberOfCellsOfIndentified);
    }

    // DOWN and LEFT
    if (row - 1 >= 0 && col + 1 < m_cols && m_flowDirection->getData(row - 1, col + 1) == EnumDirection::DownLeft && flowAccumulation.getData(row - 1, col + 1) > m_flowAccumulationLimit && !qFuzzyCompare(m_noDataValue, flowAccumulation.getData(row - 1, col + 1)))
    {
        direction.donors.emplace_back(Direction(row - 1, col + 1));
        Direction &directionDownLeft = direction.donors.back();
        numberOfCellsOfIndentified = makeTree(directionDownLeft, flowAccumulation, numberOfCellsOfIndentified);
    }

    // LEFT
    if (col + 1 < m_cols && m_flowDirection->getData(row, col + 1) == EnumDirection::Left && flowAccumulation.getData(row, col + 1) > m_flowAccumulationLimit && !qFuzzyCompare(m_noDataValue, flowAccumulation.getData(row, col + 1)))
    {
        direction.donors.emplace_back(Direction(row, col + 1));
        Direction &directionLeft = direction.donors.back();
        numberOfCellsOfIndentified = makeTree(directionLeft, flowAccumulation, numberOfCellsOfIndentified);
    }

    // UP and LEFT
    if (row + 1 < m_rows && col + 1 < m_cols && m_flowDirection->getData(row + 1, col + 1) == EnumDirection::UpLeft && flowAccumulation.getData(row + 1, col + 1) > m_flowAccumulationLimit && !qFuzzyCompare(m_noDataValue, flowAccumulation.getData(row + 1, col + 1)))
    {
        direction.donors.emplace_back(Direction(row + 1, col + 1));
        Direction &directionUpLeft = direction.donors.back();
        numberOfCellsOfIndentified = makeTree(directionUpLeft, flowAccumulation, numberOfCellsOfIndentified);
    }

    // UP
    if (row + 1 < m_rows && m_flowDirection->getData(row + 1, col) == EnumDirection::Up && flowAccumulation.getData(row + 1, col) > m_flowAccumulationLimit && !qFuzzyCompare(m_noDataValue, flowAccumulation.getData(row + 1, col)))
    {
        direction.donors.emplace_back(Direction(row + 1, col));
        Direction &directionUp = direction.donors.back();
        numberOfCellsOfIndentified = makeTree(directionUp, flowAccumulation, numberOfCellsOfIndentified);
    }

    // UP and RIGHT
    if (row + 1 < m_rows && col - 1 >= 0 && m_flowDirection->getData(row + 1, col - 1) == EnumDirection::UpRight && flowAccumulation.getData(row + 1, col - 1) > m_flowAccumulationLimit && !qFuzzyCompare(m_noDataValue, flowAccumulation.getData(row + 1, col - 1)))
    {
        direction.donors.emplace_back(Direction(row + 1, col - 1));
        Direction &directionUpRight = direction.donors.back();
        numberOfCellsOfIndentified = makeTree(directionUpRight, flowAccumulation, numberOfCellsOfIndentified);
    }

    return numberOfCellsOfIndentified;
}

void DirectionCalculatorService::makeTreeDonors(DrainageNetwork &drainageNetwork)
{
    drainageNetwork.positions.resize(drainageNetwork.numberOfCellsOfIndentified);
    if (m_processMapsPositionsAndBranches)
    {
        drainageNetwork.mapPositions = std::make_shared<Raster<short>>(
                    m_flowDirection->getRows(),
                    m_flowDirection->getCols(),
                    m_flowDirection->getXOrigin(),
                    m_flowDirection->getYOrigin(),
                    m_flowDirection->getCellSize(),
                    -9999
                );
        drainageNetwork.mapBranches = std::make_shared<Raster<short>>(
                    m_flowDirection->getRows(),
                    m_flowDirection->getCols(),
                    m_flowDirection->getXOrigin(),
                    m_flowDirection->getYOrigin(),
                    m_flowDirection->getCellSize(),
                    -9999
                );
    }
    drainageNetwork.positions[0] = {drainageNetwork.mainDirection->getRow(), drainageNetwork.mainDirection->getCol()};

    makeTreeDonors(drainageNetwork, *drainageNetwork.mainDirection, 0, 1);
}

std::pair<short, size_t> DirectionCalculatorService::makeTreeDonors(DrainageNetwork &drainageNetwork, const Direction &direction, size_t position, short numberOfTree)
{
    const size_t length = direction.donors.size();

    if (length > 0)
    {
        const size_t row = direction.donors[0].getRow();
        const size_t col = direction.donors[0].getCol();
        ++position;
        drainageNetwork.positions[position] = {row, col};
        if (m_processMapsPositionsAndBranches)
        {
            drainageNetwork.mapBranches->setData(row, col, numberOfTree);
            if (LimitMapPositions >= numberOfTree)
            {
                drainageNetwork.mapPositions->setData(direction.donors[0].getRow(), direction.donors[0].getCol(), static_cast<short>(position));
            }
        }
        const std::pair<short, size_t> result = makeTreeDonors(drainageNetwork, direction.donors[0], position, numberOfTree);
        numberOfTree = result.first;
        position = result.second;

        for (size_t i = 1; i < length; ++i)
        {
            ++numberOfTree;
            ++position;
            drainageNetwork.positions[position] = {direction.donors[i].getRow(), direction.donors[i].getCol()};
            if (m_processMapsPositionsAndBranches)
            {
                drainageNetwork.mapBranches->setData(direction.donors[i].getRow(), direction.donors[i].getCol(), numberOfTree);
                if (LimitMapPositions >= numberOfTree)
                {
                    drainageNetwork.mapPositions->setData(direction.donors[i].getRow(), direction.donors[i].getCol(), static_cast<short>(position) + 1);
                }
            }
            const std::pair<short, size_t> result = makeTreeDonors(drainageNetwork, direction.donors[i], position, numberOfTree);
            numberOfTree = result.first;
            position = result.second;
        }
    }

    return {numberOfTree, position};
}

} // namespace service
} // namespace landscapeevolutionmodel
} // namespace continental
