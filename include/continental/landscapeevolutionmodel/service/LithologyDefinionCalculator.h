/*
* Developed by UNISINOS - Projeto Continentais and Petrobras
* author: Rudi C. C. Modena
* email: rmodena@unisinos.br
* date: October, 2020
*/
#ifndef CONTINENTAL_FUZZY_SERVICE_FLUVIALSYSTEM_SHARED_LITHOLOGYDEFINITIONCALCULATOR_H
#define CONTINENTAL_FUZZY_SERVICE_FLUVIALSYSTEM_SHARED_LITHOLOGYDEFINITIONCALCULATOR_H

#define _USE_MATH_DEFINES
#include <cmath>
#include "continental/landscapeevolutionmodel/export.h"
#include <continental/datamanagement/Raster.h>
#include "continental/landscapeevolutionmodel/domain/definition/GrainNames.h"
#include "continental/landscapeevolutionmodel/constant/LandscapeEvolutionModelConstant.h"
#include "continental/landscapeevolutionmodel/service/SlopeCalculator.h"
#include <memory>

namespace continental {
namespace landscapeevolutionmodel {
namespace service {

class CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_EXPORT_DECL LithologyDefinionCalculator
{

public:
    void calculateGrainDiscretizationRaster();

    std::shared_ptr<continental::datamanagement::Raster<int>> getFlowAccumulationRaster() const;
    void setFlowAccumulationRaster(const std::shared_ptr<continental::datamanagement::Raster<int>> flowAccumulationRaster);

    std::shared_ptr<continental::datamanagement::Raster<short>> getLithologyDefinitionRaster() const;
    void setLithologyDefinitionRaster(const std::shared_ptr<continental::datamanagement::Raster<short>> lithologyDefinitionRaster);

    double getDischargeKParameter() const;
    void setDischargeKParameter(const double dischargeKParameter);

    double getDischargeEParameter() const;
    void setDischargeEParameter(const double dischargeEParameter);

    double getChannelDepthCParameter() const;
    void setChannelDepthCParameter(const double channelDepthCParameter);

    double getChannelDepthFParameter() const;
    void setChannelDepthFParameter(const double channelDepthFParameter);

    std::shared_ptr<continental::datamanagement::Raster<double> > getDemRaster() const;
    void setDemRaster(const std::shared_ptr<continental::datamanagement::Raster<double> > demRaster);

    double getGrainSizeWaterDensity() const;
    void setGrainSizeWaterDensity(const double grainSizeWaterDensity);

    double getGrainSizeSedimentDensity() const;
    void setGrainSizeSedimentDensity(const double grainSizeSedimentDensity);

    double getGrainSizeShieldsNumber() const;
    void setGrainSizeShieldsNumber(const double grainSizeShieldsNumber);

    std::shared_ptr<continental::datamanagement::Raster<double> > getSlope() const;
    void setSlope(const std::shared_ptr<continental::datamanagement::Raster<double> > slope);

    std::shared_ptr<continental::datamanagement::Raster<double> > getD50() const;
    void setD50(const std::shared_ptr<continental::datamanagement::Raster<double> > d50);

private:
    /// Membros.
    double m_dischargeKParameter = continental::landscapeevolutionmodel::constant::LandscapeEvolutionModelConstant::DischargeKParameter;
    double m_dischargeEParameter = continental::landscapeevolutionmodel::constant::LandscapeEvolutionModelConstant::DischargeEParameter;
    double m_channelDepthCParameter = continental::landscapeevolutionmodel::constant::LandscapeEvolutionModelConstant::ChannelDepthCParameter;
    double m_channelDepthFParameter = continental::landscapeevolutionmodel::constant::LandscapeEvolutionModelConstant::ChannelDepthFParameter;
    double m_grainSizeWaterDensity = continental::landscapeevolutionmodel::constant::LandscapeEvolutionModelConstant::GrainSizeWaterDensity;
    double m_grainSizeSedimentDensity = continental::landscapeevolutionmodel::constant::LandscapeEvolutionModelConstant::GrainSizeSedimentDensity;
    double m_grainSizeShieldsNumber = continental::landscapeevolutionmodel::constant::LandscapeEvolutionModelConstant::GrainSizeShieldsNumber;
    std::shared_ptr<continental::datamanagement::Raster<int>> m_flowAccumulationRaster;
    std::shared_ptr<continental::datamanagement::Raster<double>> m_demRaster;
    std::shared_ptr<continental::datamanagement::Raster<double>> m_slope;
    std::shared_ptr<continental::datamanagement::Raster<double>> m_d50;
    std::shared_ptr<continental::datamanagement::Raster<short>> m_lithologyDefinitionRaster;

    double calculateFlowAccumulationSquareMeters(const double pixelsFlowAccumulation) const;
    double calculateDischarge(const double flowAccumulationSquareMeters) const;
    double calculateChannelDepth(const double discharge) const;
    double calculateGrainSizeD50Quocient() const;
    double calculateGrainSizeD50(const double channelDepth, const double adimensionalSlope) const;
    short calculateGrainDiscretizationValue(const double grainSizeD50) const;
    double convertDegreesAdimensional(const double degreeValue) const;
    double convertAdimensionalDegrees(const double adimensionalValue) const;
};

}
}
}

#endif // CONTINENTAL_FUZZY_SERVICE_FLUVIALSYSTEM_SHARED_LITHOLOGYDEFINITIONCALCULATOR_H
