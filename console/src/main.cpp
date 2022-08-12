/*
* Developed by UNISINOS
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: March, 2020
*/
#include <iostream>
#include <iomanip>
#include <QString>
#include <QCoreApplication>
#include <continental/datamanagement/RasterFile.h>
#include "continental/landscapeevolutionmodel/ProcessLandscapeEvolutionModel.h"
#include "continental/landscapeevolutionmodel/dto/LandscapeEvolutionModelInput.h"
#include <continental/landscapeevolutionmodel/domain/HeuristicSinkRemovalProcessingMode.h>
#include "continental/landscapeevolutionmodel/domain/SimulationLandscapeEvolutionModelConfig.h"
#include "continental/landscapeevolutionmodel/domain/SinkDestroyConfig.h"
#include "continental/landscapeevolutionmodel/domain/StreamDefinitionConfig.h"
#include "continental/landscapeevolutionmodel/domain/StreamDefinitionThresholdType.h"
#include "continental/landscapeevolutionmodel/domain/GrainDispersionConfig.h"
#include "continental/landscapeevolutionmodel/constant/LandscapeEvolutionModelConstant.h"
#include <continental/landscapeevolutionmodel/util/RasterFileUtil.h>
#include <continental/landscapeevolutionmodel/service/DirectionCalculatorService.h>
#include <continental/landscapeevolutionmodel/service/LithologyDefinionCalculator.h>

using namespace continental::datamanagement;
using namespace continental::landscapeevolutionmodel;
using namespace continental::landscapeevolutionmodel::constant;
using namespace continental::landscapeevolutionmodel::dto;
using namespace continental::landscapeevolutionmodel::service;

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    QStringList args = app.arguments();

    std::cout << "INICIOU O PROCESSO DO LEM" << std::endl;

    // Dados de Entrada da superficie inicial

    const QString basePath = "C:/Git-vs2019/ContinentalLandscapeEvolutionModelMock/";



    const QString initialGridFileAfterSinkAndDestroy = "bacia_piratini_1000.asc";
    const QString initialGridPath = basePath + initialGridFileAfterSinkAndDestroy;

    std::cout << initialGridPath.toStdString() << std::endl;
    std::shared_ptr<Raster<double>> initialGrid = std::make_shared<Raster<double>>(RasterFile<double>::loadRasterByFile(initialGridPath));



    const QString upliftRatefile = "bacia_piratini_1000_uplift.asc";
    const QString upliftRatePath = basePath + upliftRatefile;
    std::shared_ptr<Raster<double>> upliftRate = std::make_shared<Raster<double>>(RasterFile<double>::loadRasterByFile(upliftRatePath));

    // lembrar de selecionar um arquivo no formato do filtro que separa as regiões subaérea e subaquosa.
    // Neste formato, os valores de 0 indicam região subaquosa e os valores de 1indicam regiões subaérea.
    const QString underwaterSeparatedFile = "bacia_piratini_1000_uplift.asc";
    const QString underwaterSeparatedPath = basePath + underwaterSeparatedFile;
    std::shared_ptr<Raster<short>> underwaterSeparatedGrid = std::make_shared<Raster<short>>(RasterFile<short>::loadRasterByFile(underwaterSeparatedPath));


    QString saveDem = "saved_teste.asc";
    QString saveDemFile = basePath + saveDem;


    auto sinkDestroyConfig = std::make_shared<domain::SinkDestroyConfig>();
    sinkDestroyConfig->setVersion(1);
    sinkDestroyConfig->setMaxOpenList(LandscapeEvolutionModelConstant::ParametersSinkDestroyConfigMaxOpenList);
    sinkDestroyConfig->setMaxClosedList(LandscapeEvolutionModelConstant::ParametersSinkDestroyConfigMaxClosedList);
    sinkDestroyConfig->setCostFunctionWeight(LandscapeEvolutionModelConstant::ParametersSinkDestroyConfigWeightFunctionCost);
    sinkDestroyConfig->setProcessingAlgorithm(domain::HeuristicSinkRemovalProcessingMode::MHS);
    
    auto streamDefinitionConfig = std::make_shared<domain::StreamDefinitionConfig>();
    streamDefinitionConfig->setThresholdType(domain::StreamDefinitionThresholdType::NumberOfCells);
    streamDefinitionConfig->setThresholdValue(2.0);

    auto simulationLandscapeEvolutionModelConfig = std::make_shared<domain::SimulationLandscapeEvolutionModelConfig>();
    simulationLandscapeEvolutionModelConfig->setErodibility(LandscapeEvolutionModelConstant::ParametersSimulationErodibility);
    simulationLandscapeEvolutionModelConfig->setDiffusivity(LandscapeEvolutionModelConstant::ParametersSimulationDiffusivity);
    simulationLandscapeEvolutionModelConfig->setConcavityIndex(LandscapeEvolutionModelConstant::ParametersSimulationValueConcavityIndex);
    simulationLandscapeEvolutionModelConfig->setValueN(LandscapeEvolutionModelConstant::ParametersSimulationValueN);
    simulationLandscapeEvolutionModelConfig->setDimensionLessPrecipitationRate(LandscapeEvolutionModelConstant::ParametersSimulationDimensionLessPrecipitationRate);
    simulationLandscapeEvolutionModelConfig->setDimensionLessDepositionCoeficient(LandscapeEvolutionModelConstant::ParametersSimulationDimensionLessDepositionCoeficient);

    simulationLandscapeEvolutionModelConfig->setEastBoundaryFactor(LandscapeEvolutionModelConstant::BoundaryConditionEast);
    simulationLandscapeEvolutionModelConfig->setWestBoundaryFactor(LandscapeEvolutionModelConstant::BoundaryConditionWest);
    simulationLandscapeEvolutionModelConfig->setNorthBoundaryFactor(LandscapeEvolutionModelConstant::BoundaryConditionNorth);
    simulationLandscapeEvolutionModelConfig->setSouthBoundaryFactor(LandscapeEvolutionModelConstant::BoundaryConditionSouth);
    simulationLandscapeEvolutionModelConfig->useDrainageNetworkAmountLimit(5);

    auto grainDispersionConfig = std::make_shared<domain::GrainDispersionConfig>();
    grainDispersionConfig->setDischargeEParameter(LandscapeEvolutionModelConstant::DischargeEParameter);
    grainDispersionConfig->setDischargeKParameter(LandscapeEvolutionModelConstant::DischargeKParameter);
    grainDispersionConfig->setChannelDepthCParameter(LandscapeEvolutionModelConstant::ChannelDepthCParameter);
    grainDispersionConfig->setChannelDepthFParameter(LandscapeEvolutionModelConstant::ChannelDepthFParameter);
    grainDispersionConfig->setGrainSizeWaterDensity(LandscapeEvolutionModelConstant::GrainSizeWaterDensity);
    grainDispersionConfig->setGrainSizeSedimentDensity(LandscapeEvolutionModelConstant::GrainSizeSedimentDensity);
    grainDispersionConfig->setGrainSizeShieldsNumber(LandscapeEvolutionModelConstant::GrainSizeShieldsNumber);

    //Parametros para execução
    auto lemInput = std::make_shared<LandscapeEvolutionModelInput>();
    lemInput->setSinkDestroyConfig(sinkDestroyConfig);
    lemInput->setStreamDefinitionConfig(streamDefinitionConfig);
    lemInput->setSimulationLandscapeEvolutionModelConfig(simulationLandscapeEvolutionModelConfig);
    lemInput->setSimulateUntilTime(LandscapeEvolutionModelConstant::SimulateUntilTime);
    lemInput->setGrainDispersionConfig(grainDispersionConfig);
    //lemInput->setEnableSurfaceLog(LandscapeEvolutionModelConstant::EnableLogs);
    lemInput->setEnableSurfaceLog(false);

    lemInput->setUpliftRate(std::make_shared<Raster<double>>
                            (initialGrid->getRows(),
                             initialGrid->getCols(),
                             initialGrid->getXOrigin(),
                             initialGrid->getYOrigin(),
                             initialGrid->getCellSize(),
                             initialGrid->getNoDataValue())); //só pra testar: colocar um if no proceess para tratar se tem uplift

    std::cout << "CONSTRUTOR DO PROCESS LEM" << std::endl;


    //Executa o lEM com iteração
    ProcessLandscapeEvolutionModel processLem;
    //processLem.prepare(initialGrid, lemInput, underwaterSeparatedGrid);
    processLem.prepare(initialGrid, lemInput, underwaterSeparatedGrid, nullptr, nullptr);

    std::cout << " SAI CONSTRUTOR DO PROCESS LEM" << std::endl;

    bool result = true;
    do
    {
        std::cout << "PROCESSANDOO PASSO DE TEMPO " << processLem.getSimulationTimeStep() << " ANOS" << std::endl;
        result = processLem.iterate();
    }
    while(result == true);

    RasterFile<double>::writeData(*initialGrid, saveDemFile, 14);

    std::cout << "ENCERROU O PROCESSO DO LEM" << std::endl;

    std::cout << "EXECUTA O MÉTODO" << std::endl;
    processLem.getTransientSurfaceWithUnderwaterFilter();
    std::cout << "EXECUTA O MÉTODO" << std::endl;

    return 0;

}
