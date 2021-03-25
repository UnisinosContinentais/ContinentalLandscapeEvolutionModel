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

    m_surface= surface;
    m_inputParameters = inputParameters;

    prepareFlowAccumulationLimit();

    m_hydroToolsAlgorithm = HydroToolsAlgorithmService(m_surface, m_inputParameters);
    m_difusionAlgorithm = DifusionAlgorithmService(m_surface, config->getDiffusivity(), m_difusionDeltaT);

    m_difusionAlgorithm.allocateTopography();

    m_hydroToolsAlgorithm.prepareDem();

    m_eroderAlgorithm.setRaster(m_surface);
    m_eroderAlgorithm.setErodibility(config->getErodibility());
    m_eroderAlgorithm.setDeltaTime(m_erosionDeltaT);
    m_eroderAlgorithm.setConcavityIndex(config->getConcavityIndex());
    m_eroderAlgorithm.setDimensionLessPrecipitationRate(config->getDimensionLessPrecipitationRate());
    m_eroderAlgorithm.setDimensionLessDepositionCoeficient(config->getDimensionLessDepositionCoeficient());
    m_eroderAlgorithm.setFlowAccumulationLimit(m_flowAccumulationLimit);
    m_eroderAlgorithm.setUplift(inputParameters->getUplift());

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

    m_timeStepCount = 0;

    if (m_enableSurfaceLog)
    {
        QString basePath = m_logSurfacePath + "/" + QString::number(m_logAge);
        QString pathParameters = basePath + "_lem_01_parameters.txt";

        qDebug() << "lem_01_parameters: " << pathParameters;

        QFile file(pathParameters);
        if (file.open(QFile::WriteOnly | QFile::Truncate))
        {
            QTextStream out(&file);

            out << "diffusivity: " << QString::number(config->getDiffusivity(), 'f', 10) << "\n";
            out << "erodibility: " << QString::number(config->getErodibility(), 'f', 10) << "\n";
            out << "concavityIndex: " << QString::number(config->getConcavityIndex(), 'f', 10) << "\n";
            out << "precipitationRate: " << QString::number(config->getDimensionLessPrecipitationRate(), 'f', 10) << "\n";
            out << "depositionCoeficient: " << QString::number(config->getDimensionLessDepositionCoeficient(), 'f', 10) << "\n";
            out << "drainageNetworkTypeLimit: ";
            switch (config->getDrainageNetworkTypeLimit())
            {
                case OnlyMain:
                    out << "OnlyMain";
                    break;
                case Percent:
                    out << "Percent";
                    break;
                case Amount:
                    out << "Amount";
                    break;
                default:
                    out << "Undefined";
                    break;
            }
            out << "drainageNetworkAmountLimit: " << QString::number(config->getDrainageNetworkAmountLimit()) << "\n";
            out << "drainageNetworkPercentLimit: " << QString::number(config->getDrainageNetworkPercentLimit()) << "\n";
            out << "simulateUntilTime: " << QString::number(m_simulateUntilTime) << "\n";
            out << "facLimit: " << QString::number(m_flowAccumulationLimit) << "\n";
        }

        QString path = basePath + "_lem_02_initialGrid.asc";
        qDebug() << "lem_02_initialGrid: " << path;
        RasterFile<double>::writeData(*m_surface, path);
    }
}

void continental::landscapeevolutionmodel::ProcessLandscapeEvolutionModel::prepareFlowAccumulationLimit()
{
    auto streamDefinitionConfig = m_inputParameters->getStreamDefinitionConfig();
    double value = streamDefinitionConfig->getThresoldValue();

    if (streamDefinitionConfig->getThresoldType() == StreamDefinitionThresholdType::PercentualOfMaximumCells)
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
    else if (streamDefinitionConfig->getThresoldType() == StreamDefinitionThresholdType::NumberOfCells)
    {
        // Retorna o próprio valor
        m_flowAccumulationLimit = static_cast<size_t>(value);
    }
    else if (streamDefinitionConfig->getThresoldType() == StreamDefinitionThresholdType::Area)
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
        }
        else
        {
            m_eroderAlgorithm.executeWithErosionDeposition();
        }
    }

    if (!qFuzzyCompare(m_difusionAlgorithm.getDiffusivity(), 0.0))
    {
        m_difusionAlgorithm.executeWithVariableBoundary(
                m_inputParameters->getSimulationLandscapeEvolutionModelConfig()->getEastBoundaryFactor(),
                m_inputParameters->getSimulationLandscapeEvolutionModelConfig()->getWestBoundaryFactor(),
                m_inputParameters->getSimulationLandscapeEvolutionModelConfig()->getSouthBoundaryFactor(),
                m_inputParameters->getSimulationLandscapeEvolutionModelConfig()->getNorthBoundaryFactor()
            );
    }

    ++m_timeStepCount;

    bool isLast = m_timeStepCount == (m_simulateUntilTime / m_difusionDeltaT);
    if (isLast)
    {
        m_hydroToolsAlgorithm.execute();

        //Dispersão de Grãos
        m_grainDispersionService.setFlowAccumulationRaster(m_hydroToolsAlgorithm.getFlowAccumulation());
        m_grainDispersionService.setDemRaster(m_surface);
        m_grainDispersionService.setChannelDepthCParameter(m_inputParameters->getGrainDispersionConfig()->getChannelDepthCParameter());
        m_grainDispersionService.setChannelDepthFParameter(m_inputParameters->getGrainDispersionConfig()->getChannelDepthFParameter());
        m_grainDispersionService.setDischargeEParameter(m_inputParameters->getGrainDispersionConfig()->getDischargeEParameter());
        m_grainDispersionService.setDischargeKParameter(m_inputParameters->getGrainDispersionConfig()->getDischargeKParameter());
        m_grainDispersionService.setGrainSizeWaterDensity(m_inputParameters->getGrainDispersionConfig()->getGrainSizeWaterDensity());
        m_grainDispersionService.setGrainSizeShieldsNumber(m_inputParameters->getGrainDispersionConfig()->getGrainSizeShieldsNumber());
        m_grainDispersionService.setGrainSizeSedimentDensity(m_inputParameters->getGrainDispersionConfig()->getGrainSizeSedimentDensity());
        m_grainDispersionService.calculateGrainDiscretizationRaster();

        m_grainDispersion = m_grainDispersionService.getLithologyDefinitionRaster();

        if (m_enableSurfaceLog)
        {
            QString basePath = m_logSurfacePath + "/" + QString::number(m_logAge);

            QString pathParameters = basePath + "_lem_01_parameters.txt";

            qDebug() << "lem_01_parameters: " << pathParameters;

            QFile file(pathParameters);
            if (file.open(QFile::WriteOnly | QFile::Truncate))
            {
                QTextStream out(&file);

                out << "Channel Depth C: " << QString::number(m_grainDispersionService.getChannelDepthCParameter(), 'f', 10) << "\n";
                out << "Channel Depth F: " << QString::number(m_grainDispersionService.getChannelDepthFParameter(), 'f', 10) << "\n";
                out << "Discharge E: " << QString::number(m_grainDispersionService.getDischargeEParameter(), 'f', 10) << "\n";
                out << "Discharge K: " << QString::number(m_grainDispersionService.getDischargeKParameter(), 'f', 10) << "\n";
                out << "Grain Size Water Density: " << QString::number(m_grainDispersionService.getGrainSizeWaterDensity(), 'f', 10) << "\n";
                out << "Grain Size Shields Number: " << QString::number(m_grainDispersionService.getGrainSizeShieldsNumber(), 'f', 10) << "\n";
                out << "Grain Size Sediment Density: " << QString::number(m_grainDispersionService.getGrainSizeSedimentDensity(), 'f', 10) << "\n";
            }

            QString pathNewGrid = basePath + "_lem_03_newGrid.asc";
            qDebug() << "lem_03_newGrid: " << pathNewGrid;
            RasterFile<double>::writeData(*m_surface, pathNewGrid);

            QString pathFlowAccumulation = basePath + "_lem_03_flowAccumulation.asc";
            qDebug() << "lem_03_flowAccumulation: " << pathNewGrid;
            RasterFile<int>::writeData(*m_grainDispersionService.getFlowAccumulationRaster(), pathFlowAccumulation);

            QString pathSlope = basePath + "_lem_03_slope.asc";
            qDebug() << "lem_03_slope: " << pathNewGrid;
            RasterFile<double>::writeData(*m_grainDispersionService.getSlope(), pathSlope);

            QString pathD50 = basePath + "_lem_03_d50.asc";
            qDebug() << "lem_03_d50: " << pathNewGrid;
            RasterFile<double>::writeData(*m_grainDispersionService.getD50(), pathD50);

            QString pathGrainDispersion = basePath + "_lem_04_grainDispersion.asc";
            qDebug() << "lem_04_grainDispersion: " << pathGrainDispersion;
            RasterFile<short>::writeData(*m_grainDispersion, pathGrainDispersion);
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
