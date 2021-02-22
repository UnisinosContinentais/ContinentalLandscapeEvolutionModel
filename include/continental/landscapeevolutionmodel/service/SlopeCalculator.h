/*
* Developed by UNISINOS - Projeto Continentais and Petrobras
* author: Rudi C. C. Modena
* email: rmodena@unisinos.br
* date: October, 2020
*/
#ifndef CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_SERVICE_SLOPECALCULATOR_H
#define CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_SERVICE_SLOPECALCULATOR_H

#define _USE_MATH_DEFINES
#include <cmath>
#include "continental/landscapeevolutionmodel/export.h"
#include "continental/landscapeevolutionmodel/domain/definition/SlopeUnits.h"
#include "continental/landscapeevolutionmodel/domain/definition/SlopeTypes.h"
#include <continental/datamanagement/Raster.h>
#include <memory>
#include <vector>

namespace continental {
namespace landscapeevolutionmodel {
namespace service {

class CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_EXPORT_DECL SlopeCalculator
{

public:
    std::shared_ptr<continental::datamanagement::Raster<double> > getInputRaster() const;
    void setInputRaster(const std::shared_ptr<continental::datamanagement::Raster<double> > inputRaster);

    std::shared_ptr<continental::datamanagement::Raster<double> > getOutputRaster() const;
    void setOutputRaster(const std::shared_ptr<continental::datamanagement::Raster<double> > outputRaster);

    continental::landscapeevolutionmodel::domain::definition::SlopeUnits getSlopeUnit() const;
    void setSlopeUnit(const continental::landscapeevolutionmodel::domain::definition::SlopeUnits unit);

    continental::landscapeevolutionmodel::domain::definition::SlopeType getSlopeType() const;
    void setSlopeType(const continental::landscapeevolutionmodel::domain::definition::SlopeType slopeType);

    void calculateSlopeRaster();

private:
    /// Membros.
    std::shared_ptr<continental::datamanagement::Raster<double>> m_inputRaster;
    std::shared_ptr<continental::datamanagement::Raster<double>> m_outputRaster;
    continental::landscapeevolutionmodel::domain::definition::SlopeUnits m_slopeUnit = continental::landscapeevolutionmodel::domain::definition::SlopeUnits::tangent;
    continental::landscapeevolutionmodel::domain::definition::SlopeType m_slopeType = continental::landscapeevolutionmodel::domain::definition::SlopeType::slopeHorn1981;

    std::shared_ptr<std::vector<std::vector<float> >> generateMovingWindow(const size_t iPosition, const size_t jPosition) const;

    float calculateSlopeHorn1981Value(const std::shared_ptr<std::vector<std::vector<float> >> movingWindow) const;

    float calculateMaxSlope(const std::shared_ptr<std::vector<std::vector<float> >> movingWindow) const;

    float convertUnit(const float slopeTangent) const;
};

}
}
}

#endif // CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_SERVICE_SLOPECALCULATOR_H
