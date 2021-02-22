#include <continental/datamanagement/RasterFile.h>
#include <continental/landscapeevolutionmodel/service/SlopeCalculator.h>
#include <continental/landscapeevolutionmodel/service/LithologyDefinionCalculator.h>
#include <continental/landscapeevolutionmodel/domain/DrainageNetwork.h>
#include <continental/landscapeevolutionmodel/domain/definition/SlopeTypes.h>
#include <continental/landscapeevolutionmodel/domain/definition/SlopeUnits.h>
#include <continental/landscapeevolutionmodel/constant/LandscapeEvolutionModelConstant.h>
#include <gtest/gtest.h>
#include <continental/landscapeevolutionmodel/service/EroderAlgorithmService.h>
#include <continental/landscapeevolutionmodel/service/DirectionCalculatorService.h>

using namespace continental::datamanagement;
using namespace continental::landscapeevolutionmodel::service;
using namespace continental::landscapeevolutionmodel::domain;
using namespace continental::landscapeevolutionmodel::domain::definition;
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
    std::vector<std::pair<QString, double>> compares = {
        {"C:/Git/ContinentalLandscapeEvolutionModelMock/bacia_piratini_90m_directions.csv", 0},
        {"C:/Git/ContinentalLandscapeEvolutionModelMock/bacia_piratini_90m_directionsOneNegative.csv", -1}
    };

    auto flowDir = std::make_shared<continental::datamanagement::Raster<short>>(RasterFile<short>::loadRasterByFile("C:/Git/ContinentalLandscapeEvolutionModelMock/bacia_piratini_90m_flowDir.asc"));
    auto flowAcc = std::make_shared<continental::datamanagement::Raster<int>>(RasterFile<int>::loadRasterByFile("C:/Git/ContinentalLandscapeEvolutionModelMock/bacia_piratini_90m_flowAcc.asc"));

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
            directionCalculator.setFacLimit(compare.second);
            directionCalculator.execute(true);

            std::vector<std::shared_ptr<DrainageNetwork>> &drainageNetworks = *directionCalculator.getDrainageNetworks();
            std::shared_ptr<DrainageNetwork> &drainageNetwork = drainageNetworks[0];

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

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    //::testing::GTEST_FLAG(filter) = "*ImplicitErosion*";
    return RUN_ALL_TESTS();
}

