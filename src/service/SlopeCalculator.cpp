#include "continental/landscapeevolutionmodel/service/SlopeCalculator.h"

using namespace continental::datamanagement;
using namespace continental::landscapeevolutionmodel::domain::definition;

namespace continental {
namespace landscapeevolutionmodel {
namespace service {

std::shared_ptr<Raster<float> > SlopeCalculator::getOutputRaster() const
{
    return m_outputRaster;
}

void SlopeCalculator::setOutputRaster(const std::shared_ptr<Raster<float> > outputRaster)
{
    m_outputRaster = outputRaster;
}

SlopeUnits SlopeCalculator::getSlopeUnit() const
{
    return m_slopeUnit;
}

void SlopeCalculator::setSlopeUnit(const SlopeUnits unit)
{
    m_slopeUnit = unit;
}

SlopeType SlopeCalculator::getSlopeType() const
{
    return m_slopeType;
}

void SlopeCalculator::setSlopeType(const SlopeType slopeType)
{
    m_slopeType = slopeType;
}

void SlopeCalculator::calculateSlopeRaster()
{
    const Raster<float> &rasterInput = *getInputRaster();

    // Informações do Raster do DEM
    size_t inputRows = rasterInput.getRows();
    size_t inputCols = rasterInput.getCols();
    double inputXOrigin = rasterInput.getXOrigin();
    double inputYOrigin = rasterInput.getYOrigin();
    double inputCellSize = rasterInput.getCellSize();
    int inputNoData = rasterInput.getNoDataValue();

    // Cria o Raster com os resultados.
    setOutputRaster(std::make_shared<Raster<float>>(inputRows,
                                                    inputCols,
                                                    inputXOrigin,
                                                    inputYOrigin,
                                                    inputCellSize,
                                                    inputNoData));
    Raster<float> &rasterOutput = *getOutputRaster();

    int auxInputCols = static_cast<int>(inputCols);
    #pragma omp parallel for
    for (int auxJ = 0; auxJ < auxInputCols; ++auxJ)
    {
        size_t j = auxJ;
        for (size_t i = 0; i < inputRows; ++i)
        {
            std::shared_ptr<std::vector<std::vector<float>>> movingWindow = generateMovingWindow(i, j);
            float slopeValue = 0.0f;
            if (getSlopeType() == SlopeType::slopeHorn1981)
            {
                slopeValue = calculateSlopeHorn1981Value(movingWindow);
            }
            else if (getSlopeType() == SlopeType::maxSlope)
            {
                slopeValue = calculateMaxSlope(movingWindow);
            }
            rasterOutput.setData(i, j, slopeValue);
        }
    }
}

std::shared_ptr<std::vector<std::vector<float> >> SlopeCalculator::generateMovingWindow(const size_t iPosition, const size_t jPosition) const
{
    // Define o tamanho da janela móvel
    int sizeWindow = 3;

    std::vector<std::vector<float>> movingWindow(sizeWindow, std::vector<float>(sizeWindow, 0.0f));

    const Raster<float> &rasterInput = *getInputRaster();

    size_t maxRows = rasterInput.getRows();
    size_t maxCols = rasterInput.getCols();

    // Calcula a tamanho da metade da janela
    int halfWindowSize = (sizeWindow - 1) / 2;

    for (int j = -halfWindowSize; j <= halfWindowSize; ++j)
    {
        for (int i = -halfWindowSize; i <= halfWindowSize; ++i)
        {
            size_t iWindowElement = i + iPosition;
            size_t jWindowElement = j + jPosition;

            // Se a linha estiver fora do Raster substituir pelo valor que pelo central da janela móvel
            if (iWindowElement < 0 || iWindowElement >= maxRows)
            {
                movingWindow[i + halfWindowSize][j + halfWindowSize] = rasterInput.getData(iPosition, jPosition);
            }
            // Se a coluna estiver fora do Raster substituir pelo valor que pelo central da janela móvel
            else if (jWindowElement < 0 || jWindowElement >= maxCols)
            {
                movingWindow[i + halfWindowSize][j + halfWindowSize] = rasterInput.getData(iPosition, jPosition);
            }
            else
            {
                movingWindow[i + halfWindowSize][j + halfWindowSize] = rasterInput.getData(iWindowElement, jWindowElement);
            }
        }
    }
    return std::make_shared<std::vector<std::vector<float> >>(movingWindow);
}

float SlopeCalculator::calculateSlopeHorn1981Value(const std::shared_ptr<std::vector<std::vector<float> >> movingWindow) const
{
    const Raster<float> &rasterInput = *getInputRaster();
    float cellSize = static_cast<float>(rasterInput.getCellSize());

    std::vector<std::vector<float>> &movingWindowVector = *movingWindow;
    float z1 = movingWindowVector[0][0];
    float z2 = movingWindowVector[0][1];
    float z3 = movingWindowVector[0][2];
    float z4 = movingWindowVector[1][0];
    float z6 = movingWindowVector[1][2];
    float z7 = movingWindowVector[2][0];
    float z8 = movingWindowVector[2][1];
    float z9 = movingWindowVector[2][2];

    // Cálculo da declividade
    float dzDx = ((z3 - z1) + (2 * (z6 - z4)) + (z9 - z7)) / (8 * cellSize);
    float dzDy = ((z9 - z3) + (2 * (z8 - z2)) + (z7 - z1)) / (8 * cellSize);
    float slopeTangent = std::sqrt(pow(dzDx, 2) + pow(dzDy, 2));

    return convertUnit(slopeTangent);
}

float SlopeCalculator::calculateMaxSlope(const std::shared_ptr<std::vector<std::vector<float> > > movingWindow) const
{
    const Raster<float> &rasterInput = *getInputRaster();
    float cellSize = static_cast<float>(rasterInput.getCellSize());

    std::vector<std::vector<float>> &movingWindowVector = *movingWindow;

    // Obter distância diagonais
    float diagonalDistance = std::sqrt(std::pow(cellSize, 2) + std::pow(cellSize, 2));

    float z5 = movingWindowVector[1][1];

    std::vector<float> SlopeNeighbours;
    SlopeNeighbours.reserve(8);

    SlopeNeighbours.push_back(std::abs(z5 - movingWindowVector[0][0]) / diagonalDistance);
    SlopeNeighbours.push_back(std::abs(z5 - movingWindowVector[0][1]) / cellSize);
    SlopeNeighbours.push_back(std::abs(z5 - movingWindowVector[0][2]) / diagonalDistance);

    SlopeNeighbours.push_back(std::abs(z5 - movingWindowVector[1][0]) / cellSize);
    SlopeNeighbours.push_back(std::abs(z5 - movingWindowVector[1][2]) / cellSize);

    SlopeNeighbours.push_back(std::abs(z5 - movingWindowVector[2][0]) / diagonalDistance);
    SlopeNeighbours.push_back(std::abs(z5 - movingWindowVector[2][1]) / cellSize);
    SlopeNeighbours.push_back(std::abs(z5 - movingWindowVector[2][2]) / diagonalDistance);

    // Obter a maior declividade
    float slopeTangent = 0.0f;

    std::vector<float>::iterator iterator;
    for (iterator = SlopeNeighbours.begin(); iterator != SlopeNeighbours.end(); ++iterator)
    {
        if (*iterator > slopeTangent)
        {
            slopeTangent = *iterator;
        }
    }

    return convertUnit(slopeTangent);
}

float SlopeCalculator::convertUnit(const float slopeTangent) const
{
    float result = 0.0f;

    // Converte para a unidade desejada
    switch (getSlopeUnit())
    {
        case SlopeUnits::tangent:
        {
            result = slopeTangent;
            break;
        }
        case SlopeUnits::percent:
        {
            result = slopeTangent * 100;
            break;
        }
        case SlopeUnits::degree:
        {
            result = std::atan(slopeTangent) * (180.0 / M_PI);
            break;
        }
        case SlopeUnits::radians:
        {
            result = std::atan(slopeTangent);
            break;
        }
    }

    return result;
}

std::shared_ptr<Raster<float> > SlopeCalculator::getInputRaster() const
{
    return m_inputRaster;
}

void SlopeCalculator::setInputRaster(const std::shared_ptr<Raster<float> > inputRaster)
{
    m_inputRaster = inputRaster;
}

}
}
}
