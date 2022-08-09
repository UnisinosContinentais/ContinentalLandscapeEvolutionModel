#include "continental/landscapeevolutionmodel/service/HydroToolsAlgorithmService.h"
#include "continental/landscapeevolutionmodel/domain/SimulationLandscapeEvolutionModelConfig.h"
#include "continental/landscapeevolutionmodel/domain/SinkDestroyConfig.h"
#include "continental/landscapeevolutionmodel/domain/StreamDefinitionConfig.h"
#include "continental/landscapeevolutionmodel/dto/LandscapeEvolutionModelInput.h"
#include <continental/datamanagement/Raster.h>
#include <continental/datamanagement/RasterFile.h>
#include <continental/hydrotools/service/HeuristicSinkRemoval.h>
#include <continental/hydrotools/service/HeuristicSinkRemovalUtil.h>
#include <continental/hydrotools/service/FlowAccumulation.h>
#include <continental/hydrotools/service/StreamDefinition.h>
#include <continental/hydrotools/service/StreamSegmentation.h>
#include <continental/hydrotools/service/Catchment.h>
#include <continental/hydrotools/service/FlowDirection.h>
#include <continental/hydrotools/domain/HydrotoolsProcessType.h>
#include <QString>
#include <memory>
#include <vector>
#include <QDebug>

using namespace continental::hydrotools;
using namespace continental::hydrotools::service;
using namespace continental::datamanagement;
using namespace continental::landscapeevolutionmodel::dto;
using namespace continental::landscapeevolutionmodel::domain;

namespace continental {
namespace landscapeevolutionmodel {
namespace service {

HydroToolsAlgorithmService::HydroToolsAlgorithmService()
{

}

HydroToolsAlgorithmService::HydroToolsAlgorithmService(std::shared_ptr<continental::datamanagement::Raster<double>> initialGrid,
                                                       std::shared_ptr<continental::landscapeevolutionmodel::dto::LandscapeEvolutionModelInput> inputParameters,
                                                       std::shared_ptr<continental::datamanagement::Raster<short>> underwaterSeparatedGrid) :
m_initialGrid(initialGrid),
m_inputParameters(inputParameters),
m_underwaterSeparatedGrid(underwaterSeparatedGrid)
{

}

const std::shared_ptr<Raster<short>> HydroToolsAlgorithmService::getFlowDirection() const
{
	return m_flowDirection;
}

const std::shared_ptr<Raster<int>> HydroToolsAlgorithmService::getFlowAccumulation() const
{
    return m_flowAccumulation;
}

const std::shared_ptr<Raster<short>> HydroToolsAlgorithmService::getStreamDefinition() const
{
	return m_streamDefinition;
}

const std::shared_ptr<Raster<short>> HydroToolsAlgorithmService::getStreamSegmentation() const
{
	return m_streamSegmentation;
}

const std::shared_ptr<Raster<short>> & HydroToolsAlgorithmService::getWaterShed() const
{
    return m_waterShed;
}

const std::shared_ptr<std::vector<std::shared_ptr<hydrotools::service::CellWatershed>>> & HydroToolsAlgorithmService::getCellsExhilarating() const
{
    return m_cellExhilarating;
}

std::shared_ptr<continental::datamanagement::Raster<short> > HydroToolsAlgorithmService::getUnderwaterSeparatedGrid() const
{
    return m_underwaterSeparatedGrid;
}


void HydroToolsAlgorithmService::prepareDem()
{
    size_t maxOpenList = m_inputParameters->getSinkDestroyConfig()->getMaxOpenList();
    size_t maxClosedList = m_inputParameters->getSinkDestroyConfig()->getMaxClosedList();
    double weightFunctionG = m_inputParameters->getSinkDestroyConfig()->getCostFunctionWeight();
    HeuristicSinkRemovalProcessingMode processingAlgorithm = m_inputParameters->getSinkDestroyConfig()->getProcessingAlgorithm();

    auto sinkDestroy = std::make_shared<HeuristicSinkRemoval<double>>(maxOpenList, maxClosedList, weightFunctionG, static_cast<HeuristicSinkRemoval<double>::ProcessingMode>(processingAlgorithm));
    sinkDestroy->setDem(m_initialGrid);
	sinkDestroy->removeSinks();
}

void HydroToolsAlgorithmService::execute()
{

    //Parametros de entrada do Sink And Destroy
    size_t maxOpenList = m_inputParameters->getSinkDestroyConfig()->getMaxOpenList();
    size_t maxClosedList = m_inputParameters->getSinkDestroyConfig()->getMaxClosedList();
    double weightFunctionG = m_inputParameters->getSinkDestroyConfig()->getCostFunctionWeight();
    HeuristicSinkRemovalProcessingMode processingAlgorithm = m_inputParameters->getSinkDestroyConfig()->getProcessingAlgorithm();

    std::shared_ptr<HeuristicSinkRemoval<double>> sinkDestroy = std::make_shared<HeuristicSinkRemoval<double>>(maxOpenList, maxClosedList, weightFunctionG, static_cast<HeuristicSinkRemoval<double>::ProcessingMode>(processingAlgorithm));

    continental::datamanagement::Raster<double> dem = *m_initialGrid; //pegando a cópia?
    std::shared_ptr<Raster<double>> demShared = std::make_shared<Raster<double>>(dem);

    //1º Executa o Sink Destroy com uma copia da superficie inicial atual
    sinkDestroy->setDem(demShared);

    //2º Remove as Depressões
	sinkDestroy->removeSinks();
	m_flowDirection = sinkDestroy->getFlowDirection();

    //3º Calcula as direções do FLuxo
	FlowAccumulation flowAccumulationCalculator;
    flowAccumulationCalculator.setFlowDirection(m_flowDirection);
    flowAccumulationCalculator.setUnderWaterFilter(m_underwaterSeparatedGrid);
    flowAccumulationCalculator.runoffUnderWater();
    m_flowAccumulation = flowAccumulationCalculator.getFlowAccumulation();

    // 4º Desenha os rios
    double thresholdValue = m_inputParameters->getStreamDefinitionConfig()->getThresholdValue();
    StreamDefinitionThresholdType thresholdType = m_inputParameters->getStreamDefinitionConfig()->getThresholdType();
    StreamDefinition streamDefinitionCalculator;
    streamDefinitionCalculator.setFlowAccumulation(m_flowAccumulation, thresholdValue, static_cast<StreamDefinition::ThresholdType>(thresholdType));
    // Executa o processo
	streamDefinitionCalculator.defineStreams();

    m_streamDefinition = streamDefinitionCalculator.getStreamDefinition();
}

void HydroToolsAlgorithmService::executeWaterShed()
{
    FlowAccumulation flowAccumulationCalculator;
    flowAccumulationCalculator.setFlowDirection(m_flowDirection);
    flowAccumulationCalculator.setUnderWaterFilter(m_underwaterSeparatedGrid);
    flowAccumulationCalculator.runoffUnderWater();

    auto flowAccumulation = flowAccumulationCalculator.getFlowAccumulation();

    Catchment catchmentCalculator;
    catchmentCalculator.setFlowDirection(m_flowDirection);
    //catchmentCalculator.setFlowAccumulation(m_flowAccumulation);
    catchmentCalculator.setFlowAccumulation(flowAccumulation);
    catchmentCalculator.findWatersheds();

    m_waterShed = catchmentCalculator.getWaterShed();
    m_cellExhilarating = catchmentCalculator.getCellsExhilarating();
}

}
}
}
