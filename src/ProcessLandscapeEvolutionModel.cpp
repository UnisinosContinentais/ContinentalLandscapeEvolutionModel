#include "continental/landscapeevolutionmodel/ProcessLandscapeEvolutionModel.h"
#include <continental/datamanagement/RasterFile.h>
#include <continental/landscapeevolutionmodel/service/HydroToolsAlgorithmService.h>
#include <continental/landscapeevolutionmodel/service/EroderAlgorithmService.h>
#include <continental/landscapeevolutionmodel/service/DifusionAlgorithmService.h>
#include <continental/landscapeevolutionmodel/dto/LandscapeEvolutionModelInput.h>
#include "continental/landscapeevolutionmodel/domain/SimulationLandscapeEvolutionModelConfig.h"
#include "continental/landscapeevolutionmodel/domain/SinkDestroyConfig.h"
#include "continental/landscapeevolutionmodel/domain/StreamDefinitionConfig.h"
#include "continental/landscapeevolutionmodel/domain/GrainDispersionConfig.h"
#include "continental/landscapeevolutionmodel/constant/LandscapeEvolutionModelConstant.h"
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

ProcessLandscapeEvolutionModel::ProcessLandscapeEvolutionModel()
{

}

void ProcessLandscapeEvolutionModel::preprare(
        std::shared_ptr<datamanagement::Raster<float>> surface,
        std::shared_ptr<LandscapeEvolutionModelInput> inputParameters
    )
{
    //Prparando parâmetros para Exucução
    m_diffusivity = inputParameters->getSimulationLandscapeEvolutionModelConfig()->getDiffusion();
    m_erodibility = inputParameters->getSimulationLandscapeEvolutionModelConfig()->getErodibility();
    m_concavityIndex = inputParameters->getSimulationLandscapeEvolutionModelConfig()->getValueM();
    m_drainagesLength = inputParameters->getSimulationLandscapeEvolutionModelConfig()->getDrainagesLength();
    m_simulateUntilTime = inputParameters->getSimulateUntilTime();
    m_age = inputParameters->getAge();

    m_enableSurfaceLog = inputParameters->getEnableSurfaceLog();
    m_logSurfacePath = inputParameters->getPathSurfaceLog();

    m_surface= surface;
    m_inputParameters = inputParameters;

    m_hydroToolsAlgorithm = HydroToolsAlgorithmService(m_surface, m_inputParameters);
    m_difusionAlgorithm = DifusionAlgorithmService(m_surface, m_simulateUntilTime, m_diffusivity, m_difusionDeltaT);

    m_difusionAlgorithm.allocateTopography();

    m_hydroToolsAlgorithm.prepareDem();

    m_eroderAlgorithm = EroderAlgorithmService(m_surface, m_erodibility, m_erosionDeltaT, m_concavityIndex);
    m_timeStepCount = 0;

    prepareFacLimit();

    if (m_enableSurfaceLog)
    {
        QString basePath = m_logSurfacePath + "/" + QString::number(m_age);
        QString pathParameters = basePath + "_lem_01_parameters.txt";

        qDebug() << "lem_01_parameters: " << pathParameters;

        QFile file(pathParameters);
        if (file.open(QFile::WriteOnly | QFile::Truncate))
        {
            QTextStream out(&file);

            out << "diffusivity: " << QString::number(m_diffusivity, 'f', 10) << "\n";
            out << "erodibility: " << QString::number(m_erodibility, 'f', 10) << "\n";
            out << "concavityIndex: " << QString::number(m_concavityIndex, 'f', 10) << "\n";
            out << "drainagesLength: " << QString::number(m_drainagesLength) << "\n";
            out << "simulateUntilTime: " << QString::number(m_simulateUntilTime) << "\n";
            out << "facLimit: " << QString::number(m_facLimit) << "\n";
        }

        QString path = basePath + "_lem_02_initialGrid.asc";
        qDebug() << "lem_02_initialGrid: " << path;
        RasterFile<float>::writeData(*m_surface, path);
    }
}

void continental::landscapeevolutionmodel::ProcessLandscapeEvolutionModel::prepareFacLimit()
{
    auto streamDefinitionConfig = m_inputParameters->getStreamDefinitionConfig();
    float value = streamDefinitionConfig->getThresoldValue();

    if (streamDefinitionConfig->getThresoldType() == StreamDefinitionThresholdType::PercentualOfMaximumCells)
    {
        auto calculateMaxValue = m_hydroToolsAlgorithm.getFlowAccumulation()->calculateMaxValue();

        //Multiplico o máximo nº de células acumuladas por um percentual
        if(value < 0.1 || value > 100)
        {
            throw;
        }
        auto aux = (value == 100) ? 0.01 : (100 - value);
        m_facLimit = (static_cast<size_t>(calculateMaxValue * aux)) / static_cast<size_t>(calculateMaxValue);
    }
    else if (streamDefinitionConfig->getThresoldType() == StreamDefinitionThresholdType::NumberOfCells)
    {
        // Retorna o próprio valor
        m_facLimit = static_cast<size_t>(value);
    }
    else if (streamDefinitionConfig->getThresoldType() == StreamDefinitionThresholdType::Area)
    {
        auto aux = static_cast<double>(value) * pow(10, 6);
        m_facLimit = static_cast<size_t>(aux / (std::pow(m_surface->getCellSize(), 2)));
    }
}

bool ProcessLandscapeEvolutionModel::iterate()
{
    qDebug() << "ContinentalLandscapeEvolutionModel timeStepCount: " << m_timeStepCount;
    validateInterate();

    m_hydroToolsAlgorithm.execute();

    m_eroderAlgorithm.setFlowAccumulation(m_hydroToolsAlgorithm.getFlowAccumulation());
    m_eroderAlgorithm.setStreamDefinition(m_hydroToolsAlgorithm.getStreamDefinition());
    m_eroderAlgorithm.setFlowDirection(m_hydroToolsAlgorithm.getFlowDirection());

    std::shared_ptr<Raster<short>> streams = m_hydroToolsAlgorithm.getStreamDefinition();
    std::shared_ptr<Raster<float>> flowAccumalation = m_hydroToolsAlgorithm.getFlowAccumulation();

    qDebug() << "6º Executa o processo de Erosão para cada sub-passo.";

    for (size_t erosionTimeStepCount = 0; erosionTimeStepCount < (m_difusionDeltaT / m_erosionDeltaT); ++erosionTimeStepCount)
    {
        bool onlyMainDrainageNetwork = (m_drainagesLength == 0); // 0 = Principal
        double drainageNetworksLenghtPercent = (m_drainagesLength == 0) ? 100 : (m_drainagesLength * 10);

        m_eroderAlgorithm.executeWithImplicitErosion(onlyMainDrainageNetwork, drainageNetworksLenghtPercent, static_cast<short>(m_facLimit));
    }
    qDebug() << "7º Executa o processo de Difusividade \n";

    m_difusionAlgorithm.executeWithVariableBoundary(
            m_inputParameters->getSimulationLandscapeEvolutionModelConfig()->getEastBoundaryFactor(),
            m_inputParameters->getSimulationLandscapeEvolutionModelConfig()->getWestBoundaryFactor(),
            m_inputParameters->getSimulationLandscapeEvolutionModelConfig()->getSouthBoundaryFactor(),
            m_inputParameters->getSimulationLandscapeEvolutionModelConfig()->getNorthBoundaryFactor()
        );

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
            QString basePath = m_logSurfacePath + "/" + QString::number(m_age);

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
            RasterFile<float>::writeData(*m_surface, pathNewGrid);

            QString pathFlowAccumulation = basePath + "_lem_03_flowAccumulation.asc";
            qDebug() << "lem_03_flowAccumulation: " << pathNewGrid;
            RasterFile<float>::writeData(*m_grainDispersionService.getFlowAccumulationRaster(), pathFlowAccumulation);

            QString pathSlope = basePath + "_lem_03_slope.asc";
            qDebug() << "lem_03_slope: " << pathNewGrid;
            RasterFile<float>::writeData(*m_grainDispersionService.getSlope(), pathSlope);

            QString pathD50 = basePath + "_lem_03_d50.asc";
            qDebug() << "lem_03_d50: " << pathNewGrid;
            RasterFile<float>::writeData(*m_grainDispersionService.getD50(), pathD50);

            QString pathGrainDispersion = basePath + "_lem_04_grainDispersion.asc";
            qDebug() << "lem_04_grainDispersion: " << pathGrainDispersion;
            RasterFile<short>::writeData(*m_grainDispersion, pathGrainDispersion);
        }
    }

    return !isLast;
}

std::shared_ptr<continental::datamanagement::Raster<float>> ProcessLandscapeEvolutionModel::getResultSimulation()
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
