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
    QString inputDemFile = "C:/Git/ContinentalLandscapeEvolutionModelMock/erosion_deposition/micro_regiao_Piratini.asc";
    QString saveDemFile = "C:/Git/ContinentalLandscapeEvolutionModelMock/erosion_deposition/micro_regiao_Piratini_result_cpp.asc";
    std::shared_ptr<Raster<double>> initialGrid = std::make_shared<Raster<double>>(RasterFile<double>::loadRasterByFile(inputDemFile));

    auto sinkDestroyConfig = std::make_shared<domain::SinkDestroyConfig>();
    sinkDestroyConfig->setVersion(1);
    sinkDestroyConfig->setMaxOpenList(1000000);
    sinkDestroyConfig->setMaxClosedList(500000);
    sinkDestroyConfig->setCostFunctionWeight(2.0);
    sinkDestroyConfig->setProcessingAlgorithm(domain::HeuristicSinkRemovalProcessingMode::MHS);
    
    auto streamDefinitionConfig = std::make_shared<domain::StreamDefinitionConfig>();
    streamDefinitionConfig->setThresoldType("NumberOfCells");
    streamDefinitionConfig->setThresoldValue(2.0);

    auto simulationLandscapeEvolutionModelConfig = std::make_shared<domain::SimulationLandscapeEvolutionModelConfig>();
    simulationLandscapeEvolutionModelConfig->setErodibility(0.00001);
    simulationLandscapeEvolutionModelConfig->setDiffusivity(0.0);
    simulationLandscapeEvolutionModelConfig->setConcavityIndex(0.4);
    simulationLandscapeEvolutionModelConfig->setValueN(1.0);
    simulationLandscapeEvolutionModelConfig->setDimensionLessPrecipitationRate(0.2);
    simulationLandscapeEvolutionModelConfig->setDimensionLessDepositionCoeficient(1.0);
    simulationLandscapeEvolutionModelConfig->setEastBoundaryFactor(0);
    simulationLandscapeEvolutionModelConfig->setWestBoundaryFactor(0);
    simulationLandscapeEvolutionModelConfig->setNorthBoundaryFactor(0);
    simulationLandscapeEvolutionModelConfig->setSouthBoundaryFactor(0);
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
    lemInput->setSimulateUntilTime(1000);
    lemInput->setGrainDispersionConfig(grainDispersionConfig);

    //Executa o lEM com iteração
    ProcessLandscapeEvolutionModel processLem;
    processLem.prepare(initialGrid, lemInput);

    bool result = true;
    do
    {
        std::cout << "PROCESSANDOO PASSO DE TEMPO " << processLem.getSimulationTimeStep() << " ANOS" << std::endl;
        result = processLem.iterate();
    }
    while(result == true);

    RasterFile<double>::writeData(*initialGrid, saveDemFile, 14);

    std::cout << "ENCERROU O PROCESSO DO LEM" << std::endl;

    return 0;

}
