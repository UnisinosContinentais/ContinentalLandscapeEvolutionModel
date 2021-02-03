#include "continental/landscapeevolutionmodel/service/LithologyDefinionCalculator.h"
#include <iostream>

using namespace continental::datamanagement;
using namespace continental::landscapeevolutionmodel::domain::definition;
using namespace continental::landscapeevolutionmodel::constant;
using namespace continental::landscapeevolutionmodel::service;

namespace continental {
namespace landscapeevolutionmodel {
namespace service {

double LithologyDefinionCalculator::calculateFlowAccumulationSquareMeters(const double pixelsFlowAccumulation) const
{
    // Calcula o valor da acumulação em metros quadrados, caso a acumulação tenha sido calcula em número de pixels.
    const Raster<float> &rasterInput = *getFlowAccumulationRaster();

    // Informações do Raster do DEM
    double inputCellSize = rasterInput.getCellSize();
    double areaPixel = std::pow(inputCellSize, 2);

    // Adicionar 1 no FlowAccumulation, pois o ContinentalHydroTools atribui o valor 0 as nascentes.

    return (pixelsFlowAccumulation + 1) * areaPixel;
}

double LithologyDefinionCalculator::calculateDischarge(const double flowAccumulationSquareMeters) const
{
    // Calcula o valor da vazão
    return getDischargeKParameter() * std::pow(flowAccumulationSquareMeters, getDischargeEParameter());
}

double LithologyDefinionCalculator::calculateChannelDepth(const double discharge) const
{
    return getChannelDepthCParameter() * std::pow(discharge, getChannelDepthFParameter());
}

double LithologyDefinionCalculator::calculateGrainSizeD50Quocient() const
{
    return getGrainSizeShieldsNumber() * (getGrainSizeSedimentDensity() - getGrainSizeWaterDensity()) / getGrainSizeWaterDensity();
}

double LithologyDefinionCalculator::calculateGrainSizeD50(const double channelDepth, const double adimensionalSlope) const
{
    return (channelDepth * adimensionalSlope) / calculateGrainSizeD50Quocient();
}

short LithologyDefinionCalculator::calculateGrainDiscretizationValue(const double grainSizeD50) const
{
    GrainNames grainName;
    // Maior que Areia Grossa
    if (grainSizeD50 > LandscapeEvolutionModelConstant::MaxCoarseSandSize)
    {
        grainName = GrainNames::Gravel;
    }
    // Areia Grossa
    else if (grainSizeD50 > LandscapeEvolutionModelConstant::MaxMediumSandSize)
    {
        grainName = GrainNames::CoarseSand;
    }
    // Areia Média
    else if (grainSizeD50 > LandscapeEvolutionModelConstant::MaxFineSandSize)
    {
        grainName = GrainNames::MediumSand;
    }
    // Areia Fina
    else if (grainSizeD50 > LandscapeEvolutionModelConstant::MaxSiltSize)
    {
        grainName = GrainNames::FineSand;
    }
    // Silte
    else if (grainSizeD50 > LandscapeEvolutionModelConstant::MaxClaySize)
    {
        grainName = GrainNames::Silt;
    }
    // Argila
    else
    {
        grainName = GrainNames::Clay;
    }

    return static_cast<short>(grainName);
}

void LithologyDefinionCalculator::calculateGrainDiscretizationRaster()
{
    const Raster<float> &flowAccumulation = *getFlowAccumulationRaster();
    const Raster<float> &dem = *getDemRaster();

    // Informações do Raster do DEM
    size_t inputRows = dem.getRows();
    size_t inputCols = dem.getCols();
    double inputXOrigin = dem.getXOrigin();
    double inputYOrigin = dem.getYOrigin();
    double inputCellSize = dem.getCellSize();
    int inputNoData = dem.getNoDataValue();

    // Calcular a declividade
    SlopeCalculator slope;
    slope.setSlopeUnit(SlopeUnits::tangent);
    slope.setSlopeType(SlopeType::slopeHorn1981);
    slope.setInputRaster(getDemRaster());
    slope.calculateSlopeRaster();
    //std::shared_ptr<Raster<float>> resultSlope = slope.getOutputRaster();
    setSlope(slope.getOutputRaster());

    // Cria o Raster do D50.
    setD50(std::make_shared<Raster<float>>(inputRows,
                                           inputCols,
                                           inputXOrigin,
                                           inputYOrigin,
                                           inputCellSize,
                                           inputNoData));
    Raster<float> &rasterD50 = *getD50();

    // Cria o Raster com os resultados.
    setLithologyDefinitionRaster(std::make_shared<Raster<short>>(inputRows,
                                                                 inputCols,
                                                                 inputXOrigin,
                                                                 inputYOrigin,
                                                                 inputCellSize,
                                                                 inputNoData));
    Raster<short> &rasterOutput = *getLithologyDefinitionRaster();

    int auxInputCols = static_cast<int>(inputCols);
    #pragma omp parallel for
    for (int auxJ = 0; auxJ < auxInputCols; ++auxJ)
    {
        size_t j = auxJ;
        for (size_t i = 0; i < inputRows; ++i)
        {
            int accumulationPixel = static_cast<int>(flowAccumulation.getData(i, j));
            double accumulationArea = calculateFlowAccumulationSquareMeters(accumulationPixel);
            double discharge = calculateDischarge(accumulationArea);
            double channelDepth = calculateChannelDepth(discharge);
            rasterD50.setData(i, j, calculateGrainSizeD50(channelDepth, m_slope->getData(i, j)));
            short grainSize = calculateGrainDiscretizationValue(rasterD50.getData(i, j));
            rasterOutput.setData(i, j, grainSize);
        }
    }
}

double LithologyDefinionCalculator::convertDegreesAdimensional(const double degreeValue) const
{
    // Converte ângulo de graus para m '\' m, que é uma medida adimensional.
    double adimensional = std::tan(degreeValue * (M_PI / 180));

    return adimensional;
}

double LithologyDefinionCalculator::convertAdimensionalDegrees(const double adimensionalValue) const
{
    // Converte declividade de m '\' m (adimensional), para graus.
    double degree = std::atan(adimensionalValue) * (180.0 / M_PI);

    return degree;
}

std::shared_ptr<Raster<float> > LithologyDefinionCalculator::getFlowAccumulationRaster() const
{
    return m_flowAccumulationRaster;
}

void LithologyDefinionCalculator::setFlowAccumulationRaster(const std::shared_ptr<Raster<float> > flowAccumulationRaster)
{
    m_flowAccumulationRaster = flowAccumulationRaster;
}

std::shared_ptr<Raster<short> > LithologyDefinionCalculator::getLithologyDefinitionRaster() const
{
    return m_lithologyDefinitionRaster;
}

void LithologyDefinionCalculator::setLithologyDefinitionRaster(const std::shared_ptr<Raster<short> > lithologyDefinitionRaster)
{
    m_lithologyDefinitionRaster = lithologyDefinitionRaster;
}

double LithologyDefinionCalculator::getDischargeKParameter() const
{
    return m_dischargeKParameter;
}

void LithologyDefinionCalculator::setDischargeKParameter(const double dischargeKParameter)
{
    m_dischargeKParameter = dischargeKParameter;
}

double LithologyDefinionCalculator::getDischargeEParameter() const
{
    return m_dischargeEParameter;
}

void LithologyDefinionCalculator::setDischargeEParameter(const double dischargeEParameter)
{
    m_dischargeEParameter = dischargeEParameter;
}

double LithologyDefinionCalculator::getChannelDepthCParameter() const
{
    return m_channelDepthCParameter;
}

void LithologyDefinionCalculator::setChannelDepthCParameter(const double channelDepthCParameter)
{
    m_channelDepthCParameter = channelDepthCParameter;
}

double LithologyDefinionCalculator::getChannelDepthFParameter() const
{
    return m_channelDepthFParameter;
}

void LithologyDefinionCalculator::setChannelDepthFParameter(const double channelDepthFParameter)
{
    m_channelDepthFParameter = channelDepthFParameter;
}

std::shared_ptr<continental::datamanagement::Raster<float>> LithologyDefinionCalculator::getDemRaster() const
{
    return m_demRaster;
}

void LithologyDefinionCalculator::setDemRaster(const std::shared_ptr<continental::datamanagement::Raster<float>> demRaster)
{
    m_demRaster = demRaster;
}

double LithologyDefinionCalculator::getGrainSizeWaterDensity() const
{
    return m_grainSizeWaterDensity;
}

void LithologyDefinionCalculator::setGrainSizeWaterDensity(const double grainSizeWaterDensity)
{
    m_grainSizeWaterDensity = grainSizeWaterDensity;
}

double LithologyDefinionCalculator::getGrainSizeSedimentDensity() const
{
    return m_grainSizeSedimentDensity;
}

void LithologyDefinionCalculator::setGrainSizeSedimentDensity(const double grainSizeSedimentDensity)
{
    m_grainSizeSedimentDensity = grainSizeSedimentDensity;
}

double LithologyDefinionCalculator::getGrainSizeShieldsNumber() const
{
    return m_grainSizeShieldsNumber;
}

void LithologyDefinionCalculator::setGrainSizeShieldsNumber(const double grainSizeShieldsNumber)
{
    m_grainSizeShieldsNumber = grainSizeShieldsNumber;
}

std::shared_ptr<continental::datamanagement::Raster<float> > LithologyDefinionCalculator::getSlope() const
{
    return m_slope;
}

void LithologyDefinionCalculator::setSlope(const std::shared_ptr<continental::datamanagement::Raster<float> > slope)
{
    m_slope = slope;
}

std::shared_ptr<continental::datamanagement::Raster<float> > LithologyDefinionCalculator::getD50() const
{
    return m_d50;
}

void LithologyDefinionCalculator::setD50(const std::shared_ptr<continental::datamanagement::Raster<float> > d50)
{
    m_d50 = d50;
}

}
}
}
