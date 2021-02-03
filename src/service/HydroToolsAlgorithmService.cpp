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

HydroToolsAlgorithmService::HydroToolsAlgorithmService(std::shared_ptr<Raster<float>> initialGrid,
                                         std::shared_ptr<LandscapeEvolutionModelInput> inputParameters) :
m_initialGrid(initialGrid),
m_inputParameters(inputParameters)
{
}

const std::shared_ptr<Raster<short>> HydroToolsAlgorithmService::getFlowDirection() const
{
	return m_flowDirection;
}

const std::shared_ptr<Raster<float>> HydroToolsAlgorithmService::getFlowAccumulation() const
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

const std::shared_ptr<Raster<short>> HydroToolsAlgorithmService::getCatchment() const
{
	return m_catchment;
}

void HydroToolsAlgorithmService::prepareDem()
{
    size_t maxOpenList = m_inputParameters->getSinkDestroyConfig()->getMaxOpenList();
    size_t maxClosedList = m_inputParameters->getSinkDestroyConfig()->getMaxClosedList();
    float weightFunctionG = m_inputParameters->getSinkDestroyConfig()->getCostFunctionWeight();
    HeuristicSinkRemovalProcessingMode processingAlgorithm = m_inputParameters->getSinkDestroyConfig()->getProcessingAlgorithm();

    auto sinkDestroy = std::make_shared<HeuristicSinkRemoval<float>>(maxOpenList, maxClosedList, weightFunctionG, static_cast<HeuristicSinkRemoval<float>::ProcessingMode>(processingAlgorithm));
    sinkDestroy->setDem(m_initialGrid);
	sinkDestroy->removeSinks();
}

void HydroToolsAlgorithmService::execute()
{

    //Parametros de entrada do Sink And Destroy
    size_t maxOpenList = m_inputParameters->getSinkDestroyConfig()->getMaxOpenList();
    size_t maxClosedList = m_inputParameters->getSinkDestroyConfig()->getMaxClosedList();
    float weightFunctionG = m_inputParameters->getSinkDestroyConfig()->getCostFunctionWeight();
    HeuristicSinkRemovalProcessingMode processingAlgorithm = m_inputParameters->getSinkDestroyConfig()->getProcessingAlgorithm();

    std::shared_ptr<HeuristicSinkRemoval<float>> sinkDestroy = std::make_shared<HeuristicSinkRemoval<float>>(maxOpenList, maxClosedList, weightFunctionG, static_cast<HeuristicSinkRemoval<float>::ProcessingMode>(processingAlgorithm));

    continental::datamanagement::Raster<float> dem = *m_initialGrid;
    std::shared_ptr<Raster<float>> demShared = std::make_shared<Raster<float>>(dem);
    qDebug() << "1º Executa o Sink Destroy com uma copia da superficie inicial atual";
    //1º Executa o Sink Destroy com uma copia da superficie inicial atual
    sinkDestroy->setDem(demShared);

    qDebug() << "2º Remove as Depressões";
    //2º Remove as Depressões
	sinkDestroy->removeSinks();
	m_flowDirection = sinkDestroy->getFlowDirection();

    qDebug() << "3º Calcula as direções do FLuxo";
    //3º Calcula as direções do FLuxo
	FlowAccumulation flowAccumulationCalculator;
    flowAccumulationCalculator.setFlowDirection(m_flowDirection);
	flowAccumulationCalculator.runoff();
    m_flowAccumulation = flowAccumulationCalculator.getFlowAccumulation();

    qDebug() << "4º Desenha os rios";
    // 4º Desenha os rios
    float thresoldValue = m_inputParameters->getStreamDefinitionConfig()->getThresoldValue();
    StreamDefinitionThresholdType thresoldType = m_inputParameters->getStreamDefinitionConfig()->getThresoldType();
    StreamDefinition streamDefinitionCalculator;
    streamDefinitionCalculator.setFlowAccumulation(m_flowAccumulation, thresoldValue, static_cast<StreamDefinition::ThresholdType>(thresoldType));
    // Executa o processo
    qDebug() << "5º Executa o processo de Definição dos Rios";
	streamDefinitionCalculator.defineStreams();

    m_streamDefinition = streamDefinitionCalculator.getStreamDefinition();
}

}
}
}
