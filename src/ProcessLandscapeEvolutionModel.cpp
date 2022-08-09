#include "continental/landscapeevolutionmodel/ProcessLandscapeEvolutionModel.h"
#include <continental/landscapeevolutionmodel/service/HydroToolsAlgorithmService.h>
#include <continental/landscapeevolutionmodel/service/EroderAlgorithmService.h>
#include <continental/landscapeevolutionmodel/service/DifusionAlgorithmService.h>
#include <continental/landscapeevolutionmodel/service/UpliftAlgorithmService.h>
#include <continental/landscapeevolutionmodel/service/SedimentaryInputService.h>
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
        std::shared_ptr<LandscapeEvolutionModelInput> inputParameters,
        std::shared_ptr<datamanagement::Raster<short>> underwaterSeparatedGrid
    )

{
    //Prparando parâmetros para Exucução
    std::shared_ptr<SimulationLandscapeEvolutionModelConfig> config = inputParameters->getSimulationLandscapeEvolutionModelConfig();

    m_simulateUntilTime = inputParameters->getSimulateUntilTime();
    m_logAge = inputParameters->getAge();

    m_enableSurfaceLog = inputParameters->getEnableSurfaceLog();
    m_logSurfacePath = inputParameters->getPathSurfaceLog();


    //m_surface recebe a superfície inicial e os processo do LEM serão processados em sobre dela
    m_surface = surface;
    m_inputParameters = inputParameters;
    m_underwaterSeparatedGrid = underwaterSeparatedGrid;

    //este grid é inicializado como uma cópia do grid que vai processar o LEM
    //assim se subtrair esta copia e o uplift do resultado do LEM, tem-se o grid de erosão e deposição
    m_onlyErosionDepositionGrid = std::make_shared<datamanagement::Raster<double>>(*m_surface);

    //é a cópia da superfície de entrada
    m_initialSurface = std::make_shared<datamanagement::Raster<double>>(*m_surface);


    //é o dado necessário para plotar a superfície transiente no stratBR
    m_transientSurfaceWithUnderwaterFilter = std::make_shared<datamanagement::Raster<double>>(*m_surface);


    if (m_enableSurfaceLog)
    {
        QString basePath = m_logSurfacePath + "/" + "ContinentalLEM_" + QString::number(m_logAge) + "_" + QString::number(m_logNode);
        ProcessLandscapeEvolutionModelLogUtil::writeSurfaceLog( "DEBUG-m_initialSurface.asc", basePath, m_initialSurface);
    }


    prepareFlowAccumulationLimit(); // método desta classe que calcula o flowAccLimit

    m_hydroToolsAlgorithm = HydroToolsAlgorithmService(m_surface, m_inputParameters, m_underwaterSeparatedGrid);
    m_difusionAlgorithm = DifusionAlgorithmService(m_surface, config->getDiffusivity(), m_difusionDeltaT);

    m_difusionAlgorithm.allocateTopography(); //aloca o espaço de m_surface em m_T da classe difusionAlgorithm

    m_hydroToolsAlgorithm.prepareDem(); //Executa o Hydrotools -> executa o syncAndDestroy

    m_upliftAlgorithm.setInitialGrid(surface);
    m_upliftAlgorithm.setNumberOfTimeSteps(m_simulateUntilTime/m_difusionDeltaT);
    m_upliftAlgorithm.setTimeStep(m_difusionDeltaT);
    m_upliftAlgorithm.setUpliftRate(inputParameters->getUpliftRate());


    m_eroderAlgorithm.setRaster(m_surface); //o raster do eroder pegou as modificações do syncAndDestroy
    m_eroderAlgorithm.setErodibility(config->getErodibility());
    m_eroderAlgorithm.setDeltaTime(m_erosionDeltaT);
    m_eroderAlgorithm.setConcavityIndex(config->getConcavityIndex());
    m_eroderAlgorithm.setDimensionLessPrecipitationRate(config->getDimensionLessPrecipitationRate());
    m_eroderAlgorithm.setDimensionLessDepositionCoeficient(config->getDimensionLessDepositionCoeficient());
    m_eroderAlgorithm.setFlowAccumulationLimit(m_flowAccumulationLimit);
    m_eroderAlgorithm.setUplift(inputParameters->getUpliftRate());

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

    m_timeStepCount = 0;

    if (m_enableSurfaceLog)
    {

        QString basePath = m_logSurfacePath + "/" + "ContinentalLEM_" + QString::number(m_logAge) + "_" + QString::number(m_logNode);
        QString pathParameters = basePath + "-prepare-01_Parameters.txt";

        qDebug() << "lem_01_parameters: " << pathParameters;
        //1
        ProcessLandscapeEvolutionModelLogUtil::writeParametersLog(pathParameters, m_flowAccumulationLimit, config, grainDispersionConfig, inputParameters);
        //2
        ProcessLandscapeEvolutionModelLogUtil::writeSurfaceLog( "-prepare-02_Grid_Inicial.asc", basePath, m_surface);

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

    //executa os métodos de erosão apenas se a erodibilidade for diferente de 0
    if (!qFuzzyCompare(m_eroderAlgorithm.getErodibility(), 0.0))
    {
        // esse for permite que a erosão possa ser executada mais vezes dentro do passso de tempo de difusão
        for (size_t erosionTimeStepCount = 0; erosionTimeStepCount < (m_difusionDeltaT / m_erosionDeltaT); ++erosionTimeStepCount)
        {
            if (qFuzzyCompare(m_eroderAlgorithm.getDimensionLessDepositionCoeficient(), 0.0))
            {
                m_eroderAlgorithm.executeWithImplicitErosion();
                //qDebug() << "executeWithImplicitErosion";
            }
            else
            {
                m_eroderAlgorithm.executeWithErosionDeposition();
                //qDebug() << "executeWithErosionDeposition";
            }
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

    m_upliftAlgorithm.applyUplift();

    ++m_timeStepCount;

    bool isLast = m_timeStepCount == (m_simulateUntilTime / m_difusionDeltaT);

    if (isLast)
    {

        m_hydroToolsAlgorithm.execute();

        //! \todo IF - LOG TESTE - REMOVER
        if (m_enableSurfaceLog)
        {
            const QString & basePath = m_logSurfacePath + "/" + "ContinentalLEM_" + QString::number(m_logAge) + "_" + QString::number(m_logNode);

            ProcessLandscapeEvolutionModelLogUtil::writeFlowAccumulationLog("ULTIMO-PASSO-03_Flow-Accumulation-Pre", basePath, m_hydroToolsAlgorithm.getFlowAccumulation());
        }

        if (m_enableSurfaceLog)
        {
            QString basePath = m_logSurfacePath + "/" + "ContinentalLEM_" + QString::number(m_logAge) + "_" + QString::number(m_logNode);
            ProcessLandscapeEvolutionModelLogUtil::writeOnlyErosionDepositionLog( QString::number(m_timeStepCount) + "-06-DEBUG_Grid_surface_ULTIMOPASSO_COM_hydroToolsAlgorith_DE_NOVO", basePath, m_surface);
        }


        //Dispersão de Grãos
        m_grainDispersionService.setFlowAccumulationRaster(m_hydroToolsAlgorithm.getFlowAccumulation());
        m_grainDispersionService.setDemRaster(m_surface);
        m_grainDispersionService.calculateGrainDiscretizationRaster();

        m_grainDispersion = m_grainDispersionService.getLithologyDefinitionRaster();

        m_totalUplift = m_upliftAlgorithm.totalUplift();

        calculateOnlyErosionDepositionGrid();

        calculateSedimentaryInput();

        if (m_enableSurfaceLog)
        {
            QString basePath = m_logSurfacePath + "/" + "ContinentalLEM_" + QString::number(m_logAge) + "_" + QString::number(m_logNode);

            //3
            ProcessLandscapeEvolutionModelLogUtil::writeFlowAccumulationLog("ULTIMO-PASSO-03_Area_de_Drenagem_do_Grid_Inicial", basePath, m_grainDispersionService.getFlowAccumulationRaster());
            //4
            //ProcessLandscapeEvolutionModelLogUtil::writeSlopeLog("ULTIMO-PASSO-04_Gradiente_do_Grid_Inicial", basePath, m_grainDispersionService.getSlope());
            //5
            ProcessLandscapeEvolutionModelLogUtil::writeD50Log("ULTIMO-PASSO-05_D50_do_Grid_Inicial", basePath, m_grainDispersionService.getD50());
            //6
            ProcessLandscapeEvolutionModelLogUtil::writeGrainDispersionLog("-06_Litologias_do_Grid_Final", basePath, m_grainDispersion);
            ProcessLandscapeEvolutionModelLogUtil::writeSurfaceLog( "ULTIMO-PASSO-7_Grid_Final", basePath, m_surface);

            ProcessLandscapeEvolutionModelLogUtil::writeOnlyErosionDepositionLog( "ULTIMO-PASSO-DEBUG_Grid_surface", basePath, m_surface);
            ProcessLandscapeEvolutionModelLogUtil::writeOnlyErosionDepositionLog( "ULTIMO-PASSO-DEBUG_Grid_Apenas_da_Erosao_Deposicao", basePath, m_onlyErosionDepositionGrid);
            ProcessLandscapeEvolutionModelLogUtil::writeSurfaceLog( "ULTIMO-PASSO-DEBUG_Total_Uplift", basePath, m_totalUplift);
            ProcessLandscapeEvolutionModelLogUtil::writeSurfaceLog( "ULTIMO-PASSO-DEBUG_Transient_Surface", basePath, m_transientSurfaceWithUnderwaterFilter);
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

void ProcessLandscapeEvolutionModel::calculateOnlyErosionDepositionGrid()
{

    size_t rows = m_onlyErosionDepositionGrid->getRows();
    size_t cols = m_onlyErosionDepositionGrid->getCols();


    for(size_t i = 0; i < rows; ++i)
    {
        for(size_t j = 0; j < cols; ++j)
        {
            m_onlyErosionDepositionGrid->setData(i, j,
                                                 m_surface->getData(i, j) -
                                                 m_onlyErosionDepositionGrid->getData(i, j) -
                                                 m_totalUplift->getData(i, j));
        }
    }

    for(size_t i = 0; i < rows; ++i)
    {
        for(size_t j = 0; j < cols; ++j)
        {
            if (m_underwaterSeparatedGrid->getData(static_cast<size_t>(i), static_cast<size_t>(j)) < 1 && 0 < i < rows-1 && 0 < j < cols-1)
            {
                m_onlyErosionDepositionGrid->setData(static_cast<size_t>(i), static_cast<size_t>(j), 0);
            }
        }
    }
}

void ProcessLandscapeEvolutionModel::calculateSedimentaryInput()
{
    m_hydroToolsAlgorithm.executeWaterShed();

    m_sedimentaryInputService.setOnlyErosionDepositionGrid(m_onlyErosionDepositionGrid);

    m_sedimentaryInputService.setSimulateUntilTime(double(m_simulateUntilTime));

    m_sedimentaryInputService.setWaterShed(m_hydroToolsAlgorithm.getWaterShed());

    m_sedimentaryInputService.setCellExhilarating(m_hydroToolsAlgorithm.getCellsExhilarating());

    m_sedimentaryInputService.execute();
}

std::shared_ptr<datamanagement::Raster<double> > ProcessLandscapeEvolutionModel::getTransientSurfaceWithUnderwaterFilter() const
{

    size_t rows = m_onlyErosionDepositionGrid->getRows();
    size_t cols = m_onlyErosionDepositionGrid->getCols();

    for(size_t i = 0; i < rows; ++i)
    {
        for(size_t j = 0; j < cols; ++j)
        {
            m_transientSurfaceWithUnderwaterFilter->setData(i, j, m_surface->getData(i, j));
        }
    }

    for(size_t i = 0; i < rows; ++i)
    {
        for(size_t j = 0; j < cols; ++j)
        {
            if (m_underwaterSeparatedGrid->getData(static_cast<size_t>(i), static_cast<size_t>(j)) < 1 && 0 < i < rows-1 && 0 < j < cols-1)
            {
                m_transientSurfaceWithUnderwaterFilter->setData(static_cast<size_t>(i), static_cast<size_t>(j), m_initialSurface->getData(static_cast<size_t>(i), static_cast<size_t>(j)));
            }
        }
    }

    return m_transientSurfaceWithUnderwaterFilter;
}

std::shared_ptr<datamanagement::Raster<double> > ProcessLandscapeEvolutionModel::getTotalUplift() const
{
    return m_totalUplift;
}

std::shared_ptr<datamanagement::Raster<double> > ProcessLandscapeEvolutionModel::getOnlyErosionDepositionGrid() const
{
    return m_onlyErosionDepositionGrid;
}

std::shared_ptr<continental::datamanagement::Raster<short>> ProcessLandscapeEvolutionModel::getWaterShed() const
{
    return m_hydroToolsAlgorithm.getWaterShed();
}

std::shared_ptr<std::vector<std::shared_ptr<domain::SedimentaryInputContent>>> ProcessLandscapeEvolutionModel::getSedimentaryInputs() const
{
    return m_sedimentaryInputService.sedimentaryInputs();
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
