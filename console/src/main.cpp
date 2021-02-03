/*
* Developed by UNISINOS
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: March, 2020
*/
#ifndef CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_CONSOLE_TEST_H
#define  CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_CONSOLE_TEST_H

#include <iostream>
#include <QString>
#include <QCoreApplication>
#include "continental/landscapeevolutionmodel/ProcessLandscapeEvolutionModel.h"
#include "continental/landscapeevolutionmodel/dto/LandscapeEvolutionModelInput.h"
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
    std::shared_ptr<Raster<short>> flowDirection = std::make_shared<Raster<short>>(RasterFile<short>::loadRasterByFile("C:/Git/ContinentalLandscapeEvolutionModelMock/bacia_piratini_90m_flowDir.asc"));
    std::shared_ptr<Raster<float>> flowAccumulation = std::make_shared<Raster<float>>(RasterFile<float>::loadRasterByFile("C:/Git/ContinentalLandscapeEvolutionModelMock/bacia_piratini_90m_flowAcc.asc"));

    DirectionCalculatorService directionCalculator(flowDirection, flowAccumulation);
    //directionCalculator.setProcessMapsPositionsAndBranches(true);
    directionCalculator.execute(false);

    std::shared_ptr<std::vector<std::shared_ptr<domain::DrainageNetwork>>> drainageNetworks = directionCalculator.getDrainageNetworks();
    //std::cout << drainageNetworks->size() << std::endl;

    for (const std::shared_ptr<domain::DrainageNetwork> &drainageNetwork : *drainageNetworks)
    {
       //RasterFile<short>::writeData(*drainageNetwork->mapBranches, "C:/Git/ContinentalLandscapeEvolutionModelMock/bacia_piratini_90m_mapBranches.asc");
       //RasterFile<short>::writeData(*drainageNetwork->mapPositions, "C:/Git/ContinentalLandscapeEvolutionModelMock/bacia_piratini_90m_mapPositions.asc");
        
        
        //std::cout << forCount++ << " " << drainageNetwork->mainDirection->getRow() << " " << drainageNetwork->mainDirection->getCol() << " " << drainageNetwork->positions.size() << std::endl;
    }

    //return 0;

    std::cout << "INICIOU O PROCESSO DO LEM  \n" << std::endl;

    //Dados de Entrada da superficie inicial
    QString inputDemFile = "C:/Users/ADM_LOCAL/Desktop/bacia_piratini_90m.asc";
    std::shared_ptr<Raster<float>> initialGrid = std::make_shared<Raster<float>>(RasterFile<float>::loadRasterByFile(inputDemFile));


    std::shared_ptr<continental::landscapeevolutionmodel::domain::SinkDestroyConfig> sinkDestroyConfig = std::make_shared<continental::landscapeevolutionmodel::domain::SinkDestroyConfig>();
    sinkDestroyConfig->setVersion(1);
    sinkDestroyConfig->setMaxOpenList(LandscapeEvolutionModelConstant::ParametersSinkDestroyConfigMaxOpenList);
    sinkDestroyConfig->setMaxClosedList(LandscapeEvolutionModelConstant::ParametersSinkDestroyConfigMaxClosedList);
    sinkDestroyConfig->setCostFunctionWeight(LandscapeEvolutionModelConstant::ParametersSinkDestroyConfigWeightFunctionCost);
    sinkDestroyConfig->setProcessingAlgorithm("MHS");

    
    std::shared_ptr<continental::landscapeevolutionmodel::domain::StreamDefinitionConfig> streamDefinitionConfig =  std::make_shared<continental::landscapeevolutionmodel::domain::StreamDefinitionConfig>();
    streamDefinitionConfig->setThresoldType("NumberOfCells");
    streamDefinitionConfig->setThresoldValue(LandscapeEvolutionModelConstant::ParametersStreamDefinitionConfigAreaThreshold);

    
    std::shared_ptr<continental::landscapeevolutionmodel::domain::SimulationLandscapeEvolutionModelConfig> simulationLandscapeEvolutionModelConfig = std::make_shared<continental::landscapeevolutionmodel::domain::SimulationLandscapeEvolutionModelConfig>();
    simulationLandscapeEvolutionModelConfig->setErodibility(LandscapeEvolutionModelConstant::ParametersSimulationErodibility);
    simulationLandscapeEvolutionModelConfig->setDiffusion(LandscapeEvolutionModelConstant::ParametersSimulationDiffusion);
    simulationLandscapeEvolutionModelConfig->setValueM(LandscapeEvolutionModelConstant::ParametersSimulationValueM);
    simulationLandscapeEvolutionModelConfig->setValueN(LandscapeEvolutionModelConstant::ParametersSimulationValueN);
    simulationLandscapeEvolutionModelConfig->setEastBoundaryFactor(0);
    simulationLandscapeEvolutionModelConfig->setWestBoundaryFactor(0);
    simulationLandscapeEvolutionModelConfig->setNorthBoundaryFactor(0);
    simulationLandscapeEvolutionModelConfig->setSouthBoundaryFactor(0);

    std::shared_ptr<continental::landscapeevolutionmodel::domain::GrainDispersionConfig> grainDispersionConfig = std::make_shared<continental::landscapeevolutionmodel::domain::GrainDispersionConfig>();
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
    
    //Executa o lEM com iteração
    continental::landscapeevolutionmodel::ProcessLandscapeEvolutionModel processLem = ProcessLandscapeEvolutionModel();
    processLem.preprare(initialGrid, lemInput);

    
    bool result = true;
    do {
        std::cout << "\n PROCESSANDOO PASSO DE TEMPO " << processLem.getSimulationTimeStep() << " ANOS \n" << std::endl;
        result = processLem.iterate();
    } while(result == true);

    std::cout << "ENCERROU O PROCESSO DO LEM \n" << std::endl;

    return 0;

}

#endif //CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_CONSOLE_TEST_H
