#include <continental/datamanagement/RasterFile.h>
#include <continental/landscapeevolutionmodel/constant/LandscapeEvolutionModelConstant.h>
#include <continental/landscapeevolutionmodel/domain/DrainageNetwork.h>
#include <continental/landscapeevolutionmodel/domain/GrainDispersionConfig.h>
#include <continental/landscapeevolutionmodel/domain/SimulationLandscapeEvolutionModelConfig.h>
#include <continental/landscapeevolutionmodel/domain/SinkDestroyConfig.h>
#include <continental/landscapeevolutionmodel/domain/StreamDefinitionConfig.h>
#include <continental/landscapeevolutionmodel/domain/StreamDefinitionThresholdType.h>
#include <continental/landscapeevolutionmodel/domain/definition/SlopeTypes.h>
#include <continental/landscapeevolutionmodel/domain/definition/SlopeUnits.h>
#include <continental/landscapeevolutionmodel/dto/LandscapeEvolutionModelInput.h>
#include <continental/landscapeevolutionmodel/service/DirectionCalculatorService.h>
#include <continental/landscapeevolutionmodel/service/EroderAlgorithmService.h>
#include <continental/landscapeevolutionmodel/service/LithologyDefinionCalculator.h>
#include <continental/landscapeevolutionmodel/service/SlopeCalculator.h>
#include <continental/landscapeevolutionmodel/ProcessLandscapeEvolutionModel.h>
#include <gtest/gtest.h>

using namespace continental::datamanagement;
using namespace continental::landscapeevolutionmodel;
using namespace continental::landscapeevolutionmodel::service;
using namespace continental::landscapeevolutionmodel::domain;
using namespace continental::landscapeevolutionmodel::domain::definition;
using namespace continental::landscapeevolutionmodel::dto;
using namespace continental::landscapeevolutionmodel::constant;


TEST(ContinentalLandscapeEvolutionModelTest, TestSlopeSmallArcGisDegree)
{
    QString filenameDem = "C:/Git/ContinentalLandscapeEvolutionModelMock/Dispersion/Small/SmallDem.asc";
    std::shared_ptr<Raster<double>> rasterDem = std::make_shared<Raster<double>>(RasterFile<double>::loadRasterByFile(filenameDem));

    QString filenameSlope = "C:/Git/ContinentalLandscapeEvolutionModelMock/Dispersion/Small/SlopeSmallArcGisDegree.asc";
    std::shared_ptr<Raster<double>> rasterSlope = std::make_shared<Raster<double>>(RasterFile<double>::loadRasterByFile(filenameSlope));

    SlopeCalculator slope;
    slope.setSlopeUnit(SlopeUnits::degree);
    slope.setSlopeType(SlopeType::slopeHorn1981);
    slope.setInputRaster(rasterDem);
    slope.calculateSlopeRaster();
    std::shared_ptr<Raster<double>> resultSlope = slope.getOutputRaster();

    // Informações do Raster
    const size_t rasterRows = rasterDem->getRows();
    const size_t rasterCols = rasterDem->getCols();

    for (size_t j = 0; j < rasterCols; ++j)
    {
        for (size_t i = 0; i < rasterRows; ++i)
        {
            ASSERT_NEAR(rasterSlope->getData(i, j), resultSlope->getData(i, j), 0.0001);
        }
    }
}

TEST(ContinentalFuzzyTest, TestSlopeSmallArcGisPercent)
{
    QString filenameDem = "C:/Git/ContinentalLandscapeEvolutionModelMock/Dispersion/Small/SmallDem.asc";
    std::shared_ptr<Raster<double>> rasterDem = std::make_shared<Raster<double>>(RasterFile<double>::loadRasterByFile(filenameDem));

    QString filenameSlope = "C:/Git/ContinentalLandscapeEvolutionModelMock/Dispersion/Small/SlopeSmallArcGisPercent.asc";
    std::shared_ptr<Raster<double>> rasterSlope = std::make_shared<Raster<double>>(RasterFile<double>::loadRasterByFile(filenameSlope));

    SlopeCalculator slope;
    slope.setSlopeUnit(SlopeUnits::percent);
    slope.setSlopeType(SlopeType::slopeHorn1981);
    slope.setInputRaster(rasterDem);
    slope.calculateSlopeRaster();
    std::shared_ptr<Raster<double>> resultSlope = slope.getOutputRaster();

    // Informações do Raster
    const size_t rasterRows = rasterDem->getRows();
    const size_t rasterCols = rasterDem->getCols();

    for (size_t j = 0; j < rasterCols; ++j)
    {
        for (size_t i = 0; i < rasterRows; ++i)
        {
            ASSERT_NEAR(rasterSlope->getData(i, j), resultSlope->getData(i, j), 0.0001);
        }
    }
}

TEST(ContinentalFuzzyTest, TestSlopeSmallPythonFlowDirDegree)
{
    QString filenameDem = "C:/Git/ContinentalLandscapeEvolutionModelMock/Dispersion/Small/SmallDem.asc";
    std::shared_ptr<Raster<double>> rasterDem = std::make_shared<Raster<double>>(RasterFile<double>::loadRasterByFile(filenameDem));

    QString filenameSlope = "C:/Git/ContinentalLandscapeEvolutionModelMock/Dispersion/Small/SlopeSmallPythonFlowDirDegree.asc";
    std::shared_ptr<Raster<double>> rasterSlope = std::make_shared<Raster<double>>(RasterFile<double>::loadRasterByFile(filenameSlope));

    SlopeCalculator slope;
    slope.setSlopeUnit(SlopeUnits::degree);
    slope.setSlopeType(SlopeType::maxSlope);
    slope.setInputRaster(rasterDem);
    slope.calculateSlopeRaster();
    std::shared_ptr<Raster<double>> resultSlope = slope.getOutputRaster();

    // Informações do Raster
    const size_t rasterRows = rasterDem->getRows();
    const size_t rasterCols = rasterDem->getCols();

    for (size_t j = 0; j < rasterCols; ++j)
    {
        for (size_t i = 0; i < rasterRows; ++i)
        {
            ASSERT_NEAR(rasterSlope->getData(i, j), resultSlope->getData(i, j), 0.0001);
        }
    }
}

TEST(ContinentalFuzzyTest, TestSlopePiratiniArcGisDegree)
{
    QString filenameDem = "C:/Git/ContinentalLandscapeEvolutionModelMock/Dispersion/Piratini/PiratiniDem.asc";
    std::shared_ptr<Raster<double>> rasterDem = std::make_shared<Raster<double>>(RasterFile<double>::loadRasterByFile(filenameDem));

    QString filenameSlope = "C:/Git/ContinentalLandscapeEvolutionModelMock/Dispersion/Piratini/SlopePiratiniArcGisDegree.asc";
    std::shared_ptr<Raster<double>> rasterSlope = std::make_shared<Raster<double>>(RasterFile<double>::loadRasterByFile(filenameSlope));

    SlopeCalculator slope;
    slope.setSlopeUnit(SlopeUnits::degree);
    slope.setSlopeType(SlopeType::slopeHorn1981);
    slope.setInputRaster(rasterDem);
    slope.calculateSlopeRaster();
    std::shared_ptr<Raster<double>> resultSlope = slope.getOutputRaster();

    // Informações do Raster
    const size_t rasterRows = rasterDem->getRows();
    const size_t rasterCols = rasterDem->getCols();

    for (size_t j = 0; j < rasterCols; ++j)
    {
        for (size_t i = 0; i < rasterRows; ++i)
        {
            ASSERT_NEAR(rasterSlope->getData(i, j), resultSlope->getData(i, j), 0.01);
        }
    }
}

TEST(ContinentalFuzzyTest, TestSlopePiratiniArcGisPercent)
{
    QString filenameDem = "C:/Git/ContinentalLandscapeEvolutionModelMock/Dispersion/Piratini/PiratiniDem.asc";
    std::shared_ptr<Raster<double>> rasterDem = std::make_shared<Raster<double>>(RasterFile<double>::loadRasterByFile(filenameDem));

    QString filenameSlope = "C:/Git/ContinentalLandscapeEvolutionModelMock/Dispersion/Piratini/SlopePiratiniArcGisPercent.asc";
    std::shared_ptr<Raster<double>> rasterSlope = std::make_shared<Raster<double>>(RasterFile<double>::loadRasterByFile(filenameSlope));

    SlopeCalculator slope;
    slope.setSlopeUnit(SlopeUnits::percent);
    slope.setSlopeType(SlopeType::slopeHorn1981);
    slope.setInputRaster(rasterDem);
    slope.calculateSlopeRaster();
    std::shared_ptr<Raster<double>> resultSlope = slope.getOutputRaster();

    // Informações do Raster
    const size_t rasterRows = rasterDem->getRows();
    const size_t rasterCols = rasterDem->getCols();

    for (size_t j = 0; j < rasterCols; ++j)
    {
        for (size_t i = 0; i < rasterRows; ++i)
        {
            ASSERT_NEAR(rasterSlope->getData(i, j), resultSlope->getData(i, j), 0.01);
        }
    }
}

TEST(ContinentalFuzzyTest, TestSlopePiratiniPythonFlowDirDegree)
{
    QString filenameDem = "C:/Git/ContinentalLandscapeEvolutionModelMock/Dispersion/Piratini/PiratiniDem.asc";
    std::shared_ptr<Raster<double>> rasterDem = std::make_shared<Raster<double>>(RasterFile<double>::loadRasterByFile(filenameDem));
    QString filenameSlope = "C:/Git/ContinentalLandscapeEvolutionModelMock/Dispersion/Piratini/SlopePiratiniPythonFlowDirDegree.asc";
    std::shared_ptr<Raster<double>> rasterSlope = std::make_shared<Raster<double>>(RasterFile<double>::loadRasterByFile(filenameSlope));

    SlopeCalculator slope;
    slope.setSlopeUnit(SlopeUnits::degree);
    slope.setSlopeType(SlopeType::maxSlope);
    slope.setInputRaster(rasterDem);
    slope.calculateSlopeRaster();
    std::shared_ptr<Raster<double>> resultSlope = slope.getOutputRaster();

    // Informações do Raster
    const size_t rasterRows = rasterDem->getRows();
    const size_t rasterCols = rasterDem->getCols();

    for (size_t j = 0; j < rasterCols; ++j)
    {
        for (size_t i = 0; i < rasterRows; ++i)
        {
            ASSERT_NEAR(rasterSlope->getData(i, j), resultSlope->getData(i, j), 0.01);
        }
    }
}

TEST(ContinentalLandscapeEvolutionModelTest, TestSmallGrainDiscretization)
{
    QString filenameDem = "C:/Git/ContinentalLandscapeEvolutionModelMock/Dispersion/Small/SmallDem.asc";
    std::shared_ptr<Raster<double>> rasterDem = std::make_shared<Raster<double>>(RasterFile<double>::loadRasterByFile(filenameDem));

    QString filenameAccumulationPixels = "C:/Git/ContinentalLandscapeEvolutionModelMock/Dispersion/Small/AccumulationPixelsSmallArcGis.asc";
    std::shared_ptr<Raster<int>> rasterAccumulationPixels = std::make_shared<Raster<int>>(RasterFile<int>::loadRasterByFile(filenameAccumulationPixels));

    QString filenameGrainName = "C:/Git/ContinentalLandscapeEvolutionModelMock/Dispersion/Small/GrainD50SmallPython.asc";
    std::shared_ptr<Raster<double>> rasterGrainName = std::make_shared<Raster<double>>(RasterFile<double>::loadRasterByFile(filenameGrainName));

    // Calcular a declividade
    SlopeCalculator slope;
    slope.setSlopeUnit(SlopeUnits::tangent);
    slope.setSlopeType(SlopeType::slopeHorn1981);
    slope.setInputRaster(rasterDem);
    slope.calculateSlopeRaster();
    std::shared_ptr<Raster<double>> resultSlope = slope.getOutputRaster();

    LithologyDefinionCalculator lithologyDefinition;
    lithologyDefinition.setFlowAccumulationRaster(rasterAccumulationPixels);
    lithologyDefinition.setDemRaster(rasterDem);
    lithologyDefinition.calculateGrainDiscretizationRaster();
    std::shared_ptr<Raster<short>> resultGrain = lithologyDefinition.getLithologyDefinitionRaster();

    // Informações do Raster
    const size_t rasterRows = rasterDem->getRows();
    const size_t rasterCols = rasterDem->getCols();

    for (size_t j = 0; j < rasterCols; ++j)
    {
        for (size_t i = 0; i < rasterRows; ++i)
        {
            ASSERT_EQ(rasterGrainName->getData(i, j), resultGrain->getData(i, j));
        }
    }
}

TEST(ContinentalLandscapeEvolutionModelTest, TestPiratiniGrainDiscretization)
{
    QString filenameDem = "C:/Git/ContinentalLandscapeEvolutionModelMock/Dispersion/Piratini/PiratiniDem.asc";
    std::shared_ptr<Raster<double>> rasterDem = std::make_shared<Raster<double>>(RasterFile<double>::loadRasterByFile(filenameDem));

    QString filenameAccumulationPixels = "C:/Git/ContinentalLandscapeEvolutionModelMock/Dispersion/Piratini/AccumulationPixelsPiratiniArcGis.asc";
    std::shared_ptr<Raster<int>> rasterAccumulationPixels = std::make_shared<Raster<int>>(RasterFile<int>::loadRasterByFile(filenameAccumulationPixels));

    QString filenameGrainName = "C:/Git/ContinentalLandscapeEvolutionModelMock/Dispersion/Piratini/GrainD50PiratiniPython.asc";
    std::shared_ptr<Raster<double>> rasterGrainName = std::make_shared<Raster<double>>(RasterFile<double>::loadRasterByFile(filenameGrainName));

    // Calcular a declividade
    SlopeCalculator slope;
    slope.setSlopeUnit(SlopeUnits::tangent);
    slope.setSlopeType(SlopeType::slopeHorn1981);
    slope.setInputRaster(rasterDem);
    slope.calculateSlopeRaster();
    std::shared_ptr<Raster<double>> resultSlope = slope.getOutputRaster();

    LithologyDefinionCalculator lithologyDefinition;
    lithologyDefinition.setFlowAccumulationRaster(rasterAccumulationPixels);
    lithologyDefinition.setDemRaster(rasterDem);
    lithologyDefinition.calculateGrainDiscretizationRaster();
    std::shared_ptr<Raster<short>> resultGrain = lithologyDefinition.getLithologyDefinitionRaster();

    // Informações do Raster
    const size_t rasterRows = rasterDem->getRows();
    const size_t rasterCols = rasterDem->getCols();

    for (size_t j = 0; j < rasterCols; ++j)
    {
        for (size_t i = 0; i < rasterRows; ++i)
        {
            ASSERT_EQ(rasterGrainName->getData(i, j), resultGrain->getData(i, j));
        }
    }
}

TEST(ContinentalLandscapeEvolutionModelTest, TestPythonSlope)
{
    QString filenameDem = "C:/Git/ContinentalLandscapeEvolutionModelMock/Dispersion/Python/python_dem_piratini_1000m.asc";
    std::shared_ptr<Raster<double>> rasterDem = std::make_shared<Raster<double>>(RasterFile<double>::loadRasterByFile(filenameDem));

    QString filenameMaxSlope = "C:/Git/ContinentalLandscapeEvolutionModelMock/Dispersion/Python/python_maxSlope_piratini_1000m.asc";
    std::shared_ptr<Raster<double>> rasterMaxSlope = std::make_shared<Raster<double>>(RasterFile<double>::loadRasterByFile(filenameMaxSlope));

    QString filenameHornSlope = "C:/Git/ContinentalLandscapeEvolutionModelMock/Dispersion/Python/python_horn_piratini_1000m.asc";
    std::shared_ptr<Raster<double>> rasterHornSlope = std::make_shared<Raster<double>>(RasterFile<double>::loadRasterByFile(filenameHornSlope));


    // Calcular a declividade Horn1981
    SlopeCalculator slope;
    slope.setSlopeUnit(SlopeUnits::tangent);
    slope.setSlopeType(SlopeType::slopeHorn1981);
    slope.setInputRaster(rasterDem);
    slope.calculateSlopeRaster();
    std::shared_ptr<Raster<double>> resultSlope = slope.getOutputRaster();

    // Informações do Raster
    const size_t rasterRows = rasterDem->getRows();
    const size_t rasterCols = rasterDem->getCols();

    for (size_t j = 0; j < rasterCols; ++j)
    {
        for (size_t i = 0; i < rasterRows; ++i)
        {
            ASSERT_NEAR(rasterHornSlope->getData(i, j), resultSlope->getData(i, j), 0.0001);
        }
    }

    // Calcular a declividade maxSlope
    slope.setSlopeUnit(SlopeUnits::tangent);
    slope.setSlopeType(SlopeType::maxSlope);
    slope.setInputRaster(rasterDem);
    slope.calculateSlopeRaster();
    resultSlope = slope.getOutputRaster();


    for (size_t j = 0; j < rasterCols; ++j)
    {
        for (size_t i = 0; i < rasterRows; ++i)
        {
            ASSERT_NEAR(rasterMaxSlope->getData(i, j), resultSlope->getData(i, j), 0.0001);
        }
    }
}

TEST(ContinentalLandscapeEvolutionModelTest, TestPythonD50)
{
    QString filenameDem = "C:/Git/ContinentalLandscapeEvolutionModelMock/Dispersion/Python/python_dem_piratini_1000m.asc";
    std::shared_ptr<Raster<double>> rasterDem = std::make_shared<Raster<double>>(RasterFile<double>::loadRasterByFile(filenameDem));

    QString filenameAccumulationPixels = "C:/Git/ContinentalLandscapeEvolutionModelMock/Dispersion/Python/python_flow_accumulation_piratini_1000m.asc";
    std::shared_ptr<Raster<int>> rasterAccumulationPixels = std::make_shared<Raster<int>>(RasterFile<int>::loadRasterByFile(filenameAccumulationPixels));

    QString filenameD50Name = "C:/Git/ContinentalLandscapeEvolutionModelMock/Dispersion/Python/python_d50_piratini_1000m.asc";
    std::shared_ptr<Raster<double>> rasterD50Name = std::make_shared<Raster<double>>(RasterFile<double>::loadRasterByFile(filenameD50Name));

    // Calcular a declividade
    SlopeCalculator slope;
    slope.setSlopeUnit(SlopeUnits::tangent);
    slope.setSlopeType(SlopeType::slopeHorn1981);
    slope.setInputRaster(rasterDem);
    slope.calculateSlopeRaster();
    std::shared_ptr<Raster<double>> resultSlope = slope.getOutputRaster();

    LithologyDefinionCalculator lithologyDefinition;
    lithologyDefinition.setFlowAccumulationRaster(rasterAccumulationPixels);
    lithologyDefinition.setDemRaster(rasterDem);
    lithologyDefinition.calculateGrainDiscretizationRaster();
    std::shared_ptr<Raster<double>> resultD50 = lithologyDefinition.getD50();

    // Informações do Raster
    const size_t rasterRows = rasterDem->getRows();
    const size_t rasterCols = rasterDem->getCols();

    for (size_t j = 1; j < (rasterCols - 1); ++j)
    {
        for (size_t i = 1; i < ( rasterRows -1); ++i)
        {
            ASSERT_NEAR(rasterD50Name->getData(i, j), resultD50->getData(i, j), 0.2);
        }
    }
}

TEST(ContinentalLandscapeEvolutionModelTest, DirectionCalculator)
{
    QString basePath = "C:/Git/ContinentalLandscapeEvolutionModelMock";

    std::vector<std::pair<QString, int>> compares = {
        {basePath + "/bacia_piratini_90m_directions.csv", 0},
        {basePath + "/bacia_piratini_90m_directionsOneNegative.csv", -1}
    };

    auto flowDir = std::make_shared<Raster<short>>(RasterFile<short>::loadRasterByFile(basePath + "/bacia_piratini_90m_flowDir.asc"));
    auto flowAcc = std::make_shared<Raster<int>>(RasterFile<int>::loadRasterByFile(basePath + "/bacia_piratini_90m_flowAcc.asc"));

    for (const auto &compare : compares)
    {
        QFile file(compare.first);

        bool fileStatus = file.open(QIODevice::ReadOnly);
        EXPECT_EQ(fileStatus, true) << "Erro ao abrir arquivo de comparação com as direções de facLimit " << compare.second << ".";

        if (fileStatus) {
            std::vector<std::pair<size_t, size_t>> positionsCompare;

            while (!file.atEnd()) {
                QByteArray line = file.readLine();
                auto parts = line.trimmed().split(';');
                positionsCompare.push_back({static_cast<size_t>(parts[0].toInt()), static_cast<size_t>(parts[1].toInt())});
            }

            DirectionCalculatorService directionCalculator(flowDir, flowAcc);
            directionCalculator.setFlowAccumulationLimit(compare.second);
            directionCalculator.useOnlyMainDrainageNetwork();
            directionCalculator.execute();

            std::shared_ptr<DrainageNetwork> &drainageNetwork = directionCalculator.getDrainageNetworks()->at(0);

            EXPECT_EQ(positionsCompare.size(), drainageNetwork->positions.size()) << "Número de posições total diferente do arquivo de facLimit " << compare.second << ".";

            size_t limit = std::min(positionsCompare.size(), drainageNetwork->positions.size());
            for (size_t i = 0; i < limit; ++i)
            {
                EXPECT_EQ(drainageNetwork->positions[i].row, positionsCompare[i].first);
                EXPECT_EQ(drainageNetwork->positions[i].col, positionsCompare[i].second);
            }
        }
    }
}

TEST(ContinentalLandscapeEvolutionModelTest, FutureDonorsSummation)
{
    const QString basePath = "C:/Git/ContinentalLandscapeEvolutionModelMock/";

    auto initialRaster = std::make_shared<Raster<double>>(RasterFile<double>::loadRasterByFile(basePath + "/micro_regiao_Piratini.asc"));
    auto flowDir = std::make_shared<Raster<short>>(RasterFile<short>::loadRasterByFile(basePath + "/micro_regiao_Piratini_flowDir.asc"));
    auto flowAcc = std::make_shared<Raster<int>>(RasterFile<int>::loadRasterByFile(basePath + "/micro_regiao_Piratini_flowAcc.asc"));
    auto donorsSummationPastCompare = std::make_shared<Raster<double>>(RasterFile<double>::loadRasterByFile(basePath + "/micro_regiao_Piratini_donors_summation_past.asc"));
    auto donorsSummationFutureCompare = std::make_shared<Raster<double>>(RasterFile<double>::loadRasterByFile(basePath + "/micro_regiao_Piratini_donors_summation_future.asc"));
    int flowAccumulationLimit = 2;

    EroderAlgorithmService eroderService;
    eroderService.setRaster(initialRaster);
    eroderService.setFlowDirection(flowDir);
    eroderService.setFlowAccumulation(flowAcc);
    
    DirectionCalculatorService directionCalculator(flowDir, flowAcc);
    directionCalculator.setFlowAccumulationLimit(flowAccumulationLimit);
    directionCalculator.useOnlyMainDrainageNetwork();
    directionCalculator.execute();

    std::shared_ptr<std::vector<std::shared_ptr<DrainageNetwork>>> drainageNetworks = directionCalculator.getDrainageNetworks();

    double epsilon = std::pow(10, -10);

    std::vector<std::vector<double>> futureDonorsSummation = eroderService.donorsSummation(drainageNetworks->at(0)->positions, false);

    for (size_t row = 0; row < futureDonorsSummation.size(); ++row)
    {
        for (size_t col = 0; col < futureDonorsSummation[0].size(); ++col)
        {
            EXPECT_LT(std::abs(futureDonorsSummation[row][col] - donorsSummationFutureCompare->getData(row, col)), epsilon);
        }
    }
}

TEST(ContinentalLandscapeEvolutionModelTest, PastDonorsSummation)
{
    const QString basePath = "C:/Git/ContinentalLandscapeEvolutionModelMock/";

    auto initialRaster = std::make_shared<Raster<double>>(RasterFile<double>::loadRasterByFile(basePath + "/micro_regiao_Piratini.asc"));
    auto flowDir = std::make_shared<Raster<short>>(RasterFile<short>::loadRasterByFile(basePath + "/micro_regiao_Piratini_flowDir.asc"));
    auto flowAcc = std::make_shared<Raster<int>>(RasterFile<int>::loadRasterByFile(basePath + "/micro_regiao_Piratini_flowAcc.asc"));
    auto donorsSummationPastCompare = std::make_shared<Raster<double>>(RasterFile<double>::loadRasterByFile(basePath + "/micro_regiao_Piratini_donors_summation_past.asc"));
    auto upliftRaster = std::make_shared<Raster<double>>(RasterFile<double>::loadRasterByFile(basePath + "/micro_regiao_Piratini_uplift.asc"));
    int flowAccumulationLimit = 2;

    EroderAlgorithmService eroderService;
    eroderService.setRaster(initialRaster);
    eroderService.setFlowDirection(flowDir);
    eroderService.setFlowAccumulation(flowAcc);
    eroderService.setUplift(upliftRaster);
    eroderService.setDeltaTime(1000);

    DirectionCalculatorService directionCalculator(flowDir, flowAcc);
    directionCalculator.setFlowAccumulationLimit(flowAccumulationLimit);
    directionCalculator.useOnlyMainDrainageNetwork();
    directionCalculator.execute();

    std::shared_ptr<std::vector<std::shared_ptr<DrainageNetwork>>> drainageNetworks = directionCalculator.getDrainageNetworks();

    double epsilon = std::pow(10, -5);

    std::vector<std::vector<double>> donorsSummationPast = eroderService.donorsSummation(drainageNetworks->at(0)->positions, true);

    for (size_t row = 0; row < donorsSummationPast.size(); ++row)
    {
        for (size_t col = 0; col < donorsSummationPast[0].size(); ++col)
        {            
            EXPECT_LT(std::abs(donorsSummationPast[row][col] - donorsSummationPastCompare->getData(row, col)), epsilon) << "row: " << row << " col: " << col << "value: " << donorsSummationPast[row][col] << " compare: " << donorsSummationPastCompare->getData(row, col);
        }
    }
}

TEST(ContinentalLandscapeEvolutionModelTest, ErosionDeposition)
{
    const QString basePath = "C:/Git/ContinentalLandscapeEvolutionModelMock/";

    auto initialRaster = std::make_shared<Raster<double>>(RasterFile<double>::loadRasterByFile(basePath + "/micro_regiao_Piratini.asc"));
    auto flowDir = std::make_shared<Raster<short>>(RasterFile<short>::loadRasterByFile(basePath + "/micro_regiao_Piratini_flowDir.asc"));
    auto flowAcc = std::make_shared<Raster<int>>(RasterFile<int>::loadRasterByFile(basePath + "/micro_regiao_Piratini_flowAcc.asc"));
    auto result = std::make_shared<Raster<double>>(RasterFile<double>::loadRasterByFile(basePath + "/micro_regiao_Piratini_result.asc"));
    auto upliftRaster = std::make_shared<Raster<double>>(RasterFile<double>::loadRasterByFile(basePath + "/micro_regiao_Piratini_uplift.asc"));

    EroderAlgorithmService eroderService;
    eroderService.setRaster(initialRaster);
    eroderService.setFlowDirection(flowDir);
    eroderService.setFlowAccumulation(flowAcc);
    eroderService.setErodibility(0.00001);
    eroderService.setConcavityIndex(0.4);
    eroderService.setDimensionLessDepositionCoeficient(1);
    eroderService.setDimensionLessPrecipitationRate(0.2);
    eroderService.setDeltaTime(1000);
    eroderService.setFlowAccumulationLimit(2);
    eroderService.useDrainageNetworkAmountLimit(5);
    eroderService.setUplift(upliftRaster);
    eroderService.executeWithErosionDeposition();
    
    RasterFile<double>::writeData(*eroderService.getRaster(), basePath + "/micro_regiao_Piratini_result_cpp.asc");
}

TEST(ContinentalLandscapeEvolutionModelTest, ProcessLandscapeEvolutionModel)
{
    // Dados de Entrada da superficie inicial
    QString basePath = "C:/Git/ContinentalLandscapeEvolutionModelMock/erosion_deposition";
    QString initialGridPath = basePath + "/micro_regiao_Piratini.asc";
    QString resultComparePath = basePath + "/micro_regiao_Piratini_result_python.asc";
    std::shared_ptr<Raster<double>> initialGrid = std::make_shared<Raster<double>>(RasterFile<double>::loadRasterByFile(initialGridPath));
    Raster<double> resultCompare = RasterFile<double>::loadRasterByFile(resultComparePath);

    auto sinkDestroyConfig = std::make_shared<SinkDestroyConfig>();
    sinkDestroyConfig->setVersion(1);
    sinkDestroyConfig->setMaxOpenList(1000000);
    sinkDestroyConfig->setMaxClosedList(500000);
    sinkDestroyConfig->setCostFunctionWeight(2.0);
    sinkDestroyConfig->setProcessingAlgorithm(HeuristicSinkRemovalProcessingMode::MHS);

    auto streamDefinitionConfig = std::make_shared<StreamDefinitionConfig>();
    streamDefinitionConfig->setThresholdType(StreamDefinitionThresholdType::NumberOfCells);
    streamDefinitionConfig->setThresholdValue(2.0);

    auto simulationLandscapeEvolutionModelConfig = std::make_shared<SimulationLandscapeEvolutionModelConfig>();
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

    auto grainDispersionConfig = std::make_shared<GrainDispersionConfig>();
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
        result = processLem.iterate();
    }
    while(result == true);

    double epsilon = std::pow(10, -10);
    for (size_t row = 0; row < resultCompare.getRows(); ++row)
    {
        for (size_t col = 0; col < resultCompare.getCols(); ++col)
        {
            double value = processLem.getResultSimulation()->getData(row, col);
            double valueCompare = resultCompare.getData(row, col);
            EXPECT_LT(std::abs(value - valueCompare), epsilon)
                    << "row: " << row << " col: " << col << "value: " << value << " compare: " << valueCompare;
        }
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    testing::GTEST_FLAG(filter) = "*ProcessLandscapeEvolutionModel*";
    return RUN_ALL_TESTS();
}

