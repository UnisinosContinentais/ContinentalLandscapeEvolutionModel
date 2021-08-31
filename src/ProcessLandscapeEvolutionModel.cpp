#include "continental/landscapeevolutionmodel/ProcessLandscapeEvolutionModel.h"
#include <continental/landscapeevolutionmodel/service/HydroToolsAlgorithmService.h>
#include <continental/landscapeevolutionmodel/service/EroderAlgorithmService.h>
#include <continental/landscapeevolutionmodel/service/DifusionAlgorithmService.h>
#include <continental/landscapeevolutionmodel/dto/LandscapeEvolutionModelInput.h>
#include "continental/landscapeevolutionmodel/domain/SimulationLandscapeEvolutionModelConfig.h"
#include "continental/landscapeevolutionmodel/domain/SinkDestroyConfig.h"
#include "continental/landscapeevolutionmodel/domain/StreamDefinitionConfig.h"
#include "continental/landscapeevolutionmodel/domain/GrainDispersionConfig.h"
#include "continental/landscapeevolutionmodel/constant/LandscapeEvolutionModelConstant.h"
#include <continental/datamanagement/RasterFile.h>
#include "continental/landscapeevolutionmodel/util/ProcessLandscapeEvolutionModelLogUtil.h"
#include <memory>
#include <QString>
#include <QDebug>
#include <iostream>
#include <vector>

using namespace continental::datamanagement;
using namespace continental::landscapeevolutionmodel::constant;
using namespace continental::landscapeevolutionmodel::domain;
using namespace continental::landscapeevolutionmodel::dto;
using namespace continental::landscapeevolutionmodel::service;
using namespace continental::landscapeevolutionmodel::util;

namespace continental {
namespace landscapeevolutionmodel {
void ProcessLandscapeEvolutionModel::prepare(
        std::shared_ptr<datamanagement::Raster<double>> surface,
        std::shared_ptr<LandscapeEvolutionModelInput> inputParameters
    )
{
    //Prparando parâmetros para Exucução
    std::shared_ptr<SimulationLandscapeEvolutionModelConfig> config = inputParameters->getSimulationLandscapeEvolutionModelConfig();

    m_simulateUntilTime = inputParameters->getSimulateUntilTime();
    m_logAge = inputParameters->getAge();

    m_enableSurfaceLog = inputParameters->getEnableSurfaceLog();
    m_logSurfacePath = inputParameters->getPathSurfaceLog();

    m_surface = surface;
    m_inputParameters = inputParameters;

    prepareFlowAccumulationLimit(); // método desta classe que calcula o flowAccLimit

    m_hydroToolsAlgorithm = HydroToolsAlgorithmService(m_surface, m_inputParameters);
    m_difusionAlgorithm = DifusionAlgorithmService(m_surface, config->getDiffusivity(), m_difusionDeltaT);

    m_difusionAlgorithm.allocateTopography(); //aloca o espaço de m_surface em m_T da classe difusionAlgorithm

    m_hydroToolsAlgorithm.prepareDem(); //Executa o Hydrotools -> executa o syncAndDestroy

    m_eroderAlgorithm.setRaster(m_surface); //o raster do eroder pegou as modificações do syncAndDestroy
    m_eroderAlgorithm.setErodibility(config->getErodibility());
    m_eroderAlgorithm.setDeltaTime(m_erosionDeltaT);
    m_eroderAlgorithm.setConcavityIndex(config->getConcavityIndex());
    m_eroderAlgorithm.setDimensionLessPrecipitationRate(config->getDimensionLessPrecipitationRate());
    m_eroderAlgorithm.setDimensionLessDepositionCoeficient(config->getDimensionLessDepositionCoeficient());
    m_eroderAlgorithm.setFlowAccumulationLimit(m_flowAccumulationLimit);
    m_eroderAlgorithm.setUplift(inputParameters->getUplift());

    auto grainDispersionConfig = m_inputParameters->getGrainDispersionConfig();
    m_grainDispersionService.setChannelDepthCParameter(grainDispersionConfig->getChannelDepthCParameter());
    m_grainDispersionService.setChannelDepthFParameter(grainDispersionConfig->getChannelDepthFParameter());
    m_grainDispersionService.setDischargeEParameter(grainDispersionConfig->getDischargeEParameter());
    m_grainDispersionService.setDischargeKParameter(grainDispersionConfig->getDischargeKParameter());
    m_grainDispersionService.setGrainSizeWaterDensity(grainDispersionConfig->getGrainSizeWaterDensity());
    m_grainDispersionService.setGrainSizeShieldsNumber(grainDispersionConfig->getGrainSizeShieldsNumber());
    m_grainDispersionService.setGrainSizeSedimentDensity(grainDispersionConfig->getGrainSizeSedimentDensity());

    switch (config->getDrainageNetworkTypeLimit())
    {
        case OnlyMain:
            m_eroderAlgorithm.useOnlyMainDrainageNetwork();
            break;
        case Amount:
            m_eroderAlgorithm.useDrainageNetworkAmountLimit(config->getDrainageNetworkAmountLimit());
            break;
        case Percent:
            m_eroderAlgorithm.useDrainageNetworkPercentLimit(config->getDrainageNetworkPercentLimit());
            break;
        default:
            throw std::runtime_error("The limit of the drainage networks has not been defined.");
    }

    // ???
    m_timeStepCount = 0;

    if (m_enableSurfaceLog)
    {
        m_logSurfacePath = "C:/Users/ADM_LOCAL/Desktop/teste_logs"; //precisa ver: esse cara só fica liberado para o teste

        QString basePath = m_logSurfacePath + "/" + "ContinentalLEM_" + QString::number(m_logAge) + "_" + QString::number(m_logNode);
        QString pathParameters = basePath + "-01_Parameters.txt";

        qDebug() << "lem_01_parameters: " << pathParameters;
        //1
        ProcessLandscapeEvolutionModelLogUtil::writeParametersLog(pathParameters, m_flowAccumulationLimit, config, grainDispersionConfig, inputParameters);
        //2
        ProcessLandscapeEvolutionModelLogUtil::writeSurfaceLog( "-02_Grid_Inicial.asc", basePath, m_surface);

    }
}

void continental::landscapeevolutionmodel::ProcessLandscapeEvolutionModel::prepareFlowAccumulationLimit()
{
    auto streamDefinitionConfig = m_inputParameters->getStreamDefinitionConfig();
    double value = streamDefinitionConfig->getThresholdValue();

    if (streamDefinitionConfig->getThresholdType() == StreamDefinitionThresholdType::PercentualOfMaximumCells)
    {
        auto calculateMaxValue = m_hydroToolsAlgorithm.getFlowAccumulation()->calculateMaxValue();

        //Multiplico o máximo nº de células acumuladas por um percentual
        if(value < 0.1 || value > 100)
        {
            throw;
        }
        auto aux = (value == 100) ? 0.01 : (100 - value);
        m_flowAccumulationLimit = (static_cast<size_t>(calculateMaxValue * aux)) / static_cast<size_t>(calculateMaxValue);
    }
    else if (streamDefinitionConfig->getThresholdType() == StreamDefinitionThresholdType::NumberOfCells)
    {
        // Retorna o próprio valor
        m_flowAccumulationLimit = static_cast<size_t>(value);
    }
    else if (streamDefinitionConfig->getThresholdType() == StreamDefinitionThresholdType::Area)
    {
        auto aux = static_cast<double>(value) * pow(10, 6);
        m_flowAccumulationLimit = static_cast<size_t>(aux / (std::pow(m_surface->getCellSize(), 2)));
    }
}

bool ProcessLandscapeEvolutionModel::iterate()
{
    validateInterate();

    m_hydroToolsAlgorithm.execute();

    m_eroderAlgorithm.setFlowAccumulation(m_hydroToolsAlgorithm.getFlowAccumulation());
    m_eroderAlgorithm.setStreamDefinition(m_hydroToolsAlgorithm.getStreamDefinition());
    m_eroderAlgorithm.setFlowDirection(m_hydroToolsAlgorithm.getFlowDirection());

    for (size_t erosionTimeStepCount = 0; erosionTimeStepCount < (m_difusionDeltaT / m_erosionDeltaT); ++erosionTimeStepCount)
    {
        if (qFuzzyCompare(m_eroderAlgorithm.getDimensionLessDepositionCoeficient(), 0.0))
        {
            m_eroderAlgorithm.executeWithImplicitErosion();
            qDebug() << "executeWithImplicitErosion";
        }
        else
        {
            m_eroderAlgorithm.executeWithErosionDeposition();
            qDebug() << "executeWithErosionDeposition";
        }
    }

    // não é dentro do for ?
    if (!qFuzzyCompare(m_difusionAlgorithm.getDiffusivity(), 0.0))
    {
        m_difusionAlgorithm.executeWithVariableBoundary(
                m_inputParameters->getSimulationLandscapeEvolutionModelConfig()->getEastBoundaryFactor(),
                m_inputParameters->getSimulationLandscapeEvolutionModelConfig()->getWestBoundaryFactor(),
                m_inputParameters->getSimulationLandscapeEvolutionModelConfig()->getSouthBoundaryFactor(),
                m_inputParameters->getSimulationLandscapeEvolutionModelConfig()->getNorthBoundaryFactor()
            );
        qDebug() << "executeWithVariableBoundary";
    }

    ++m_timeStepCount;

    bool isLast = m_timeStepCount == (m_simulateUntilTime / m_difusionDeltaT);
    if (isLast)
    {
        m_hydroToolsAlgorithm.execute();

        //Dispersão de Grãos
        m_grainDispersionService.setFlowAccumulationRaster(m_hydroToolsAlgorithm.getFlowAccumulation());
        m_grainDispersionService.setDemRaster(m_surface);
        m_grainDispersionService.calculateGrainDiscretizationRaster();

        m_grainDispersion = m_grainDispersionService.getLithologyDefinitionRaster();

        if (m_enableSurfaceLog)
        {
            QString basePath = m_logSurfacePath + "/" + "ContinentalLEM_" + QString::number(m_logAge) + "_" + QString::number(m_logNode);

            //3
            ProcessLandscapeEvolutionModelLogUtil::writeFlowAccumulationLog("-03_Area_de_Drenagem_do_Grid_Inicial", basePath, m_grainDispersionService.getFlowAccumulationRaster());
            //4
            ProcessLandscapeEvolutionModelLogUtil::writeSlopeLog("-04_Gradiente_do_Grid_Inicial", basePath, m_grainDispersionService.getSlope());
            //5
            ProcessLandscapeEvolutionModelLogUtil::writeD50Log("-05_D50_do_Grid_Inicial", basePath, m_grainDispersionService.getD50());
            //6
            ProcessLandscapeEvolutionModelLogUtil::writeGrainDispersionLog("-06_Litologias_do_Grid_Final", basePath, m_grainDispersion);
            //7
            ProcessLandscapeEvolutionModelLogUtil::writeSurfaceLog( "-07_Grid_Final", basePath, m_surface);
        }
    }

    return !isLast;
}

std::shared_ptr<continental::datamanagement::Raster<double>> ProcessLandscapeEvolutionModel::getResultSimulation()
{
    return m_surface;
}

std::shared_ptr<continental::datamanagement::Raster<short>> ProcessLandscapeEvolutionModel::getResultDispersionSimulation()
{
    return m_grainDispersion;
}

size_t ProcessLandscapeEvolutionModel::getSimulationTimeStep()
{
    return m_timeStepCount * m_difusionDeltaT;
}

void ProcessLandscapeEvolutionModel::validateInterate()
{
    if(m_inputParameters == nullptr || m_surface == nullptr)
    {
        throw ("Não foi possivél localizar os parâmetros de entrada para o processo Landscape Evolution Model");
    }
    if((m_timeStepCount > m_simulateUntilTime / m_difusionDeltaT))
    {
        throw ("Não foi possivél processar o passo de tempo");
    }
}


}//landscapeevolutionmodel
}//continental
