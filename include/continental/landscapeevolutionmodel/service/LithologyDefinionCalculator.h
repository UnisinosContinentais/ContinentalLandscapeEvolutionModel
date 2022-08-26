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

///
/// \brief The LithologyDefinionCalculator class Classe referente as definições
/// do cálculo das litologias
///
class CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_EXPORT_DECL LithologyDefinionCalculator
{

public:

    /// Construtor
    void calculateGrainDiscretizationRaster();

    /// Função de retorno do valor Raster do fluxo acumulado
    /// \return Retorna o valor Raster do fluxo acumulado
    std::shared_ptr<continental::datamanagement::Raster<int>> getFlowAccumulationRaster() const;

    /// Função de definição do valor Raster do fluxo acumulado
    /// \param flowAccumulationRaster Valor Raster do fluxo acumulado
    void setFlowAccumulationRaster(const std::shared_ptr<continental::datamanagement::Raster<int>> flowAccumulationRaster);

    /// Função de retorno do valor Raster da definição da litologia
    /// \return Retorna o valor Raster da definição da litologia
    std::shared_ptr<continental::datamanagement::Raster<short>> getLithologyDefinitionRaster() const;

    /// Função de definição Raster da litologia
    /// \param lithologyDefinitionRaster Valor da definição Raster da litologia
    void setLithologyDefinitionRaster(const std::shared_ptr<continental::datamanagement::Raster<short>> lithologyDefinitionRaster);

    /// Função de retorno do parametro referente ao índice de escoamento K
    /// \return Retorna o valor do índice de escoamento K
    double getDischargeKParameter() const;

    /// Função de definição do índice de escoamento K
    /// \param dischargeKParameter Valor do índice de escoamento K
    void setDischargeKParameter(const double dischargeKParameter);

    /// Função de retorno da relação de escolamento E
    /// \return Retorna o valor da relação de escolamento E
    double getDischargeEParameter() const;

    /// Função de definição da relação de escolamento E
    /// \param dischargeEParameter Valor da relação de escolamento E
    void setDischargeEParameter(const double dischargeEParameter);

    /// Função de retorno do índice C referente a geometria do canal
    /// \return Retorna o valor do índice C referente a geometria do canal
    double getChannelDepthCParameter() const;

    /// Função de definição do índice C referente a geometria do canal
    /// \param channelDepthCParameter Valor do índice C referente a geometria do canal
    void setChannelDepthCParameter(const double channelDepthCParameter);

    /// Função de retorno do índice F referente a geometria do canal
    /// \return Retorna o valor do índice F referente a geometria do canal
    double getChannelDepthFParameter() const;

    /// Função de definição do índice F referente a geometria do canal
    /// \param channelDepthFParameter Valor do índice F referente a geometria do canal
    void setChannelDepthFParameter(const double channelDepthFParameter);

    /// Função de retorno do Raster Dem
    /// \return Retorna o valor do Raster Dem
    std::shared_ptr<continental::datamanagement::Raster<double> > getDemRaster() const;

    /// Função do definição do Raster Dem
    /// \param demRaster Valor do Raster Dem
    void setDemRaster(const std::shared_ptr<continental::datamanagement::Raster<double> > demRaster);

    /// Função de retorno do valor da densidade do fluido
    /// \return Retorna o valor da densidade do fluido
    double getGrainSizeWaterDensity() const;

    /// Função de definição da densidade do fluido
    /// \param grainSizeWaterDensity Valor da densidade do fluido
    void setGrainSizeWaterDensity(const double grainSizeWaterDensity);

    /// Função de retorno da densidade do sedimento
    /// \return Retorna o valor da densidade do sedimento
    double getGrainSizeSedimentDensity() const;

    /// Função de definição da densidade do sedimento
    /// \param grainSizeSedimentDensity Valor da densidade do sedimento
    void setGrainSizeSedimentDensity(const double grainSizeSedimentDensity);

    /// Função de retorno do valor do número de shields
    /// \return Retorna o valor do número de shields
    double getGrainSizeShieldsNumber() const;

    /// Função de definição do valor do número de shields
    /// \param grainSizeShieldsNumber Valor do número de shields
    void setGrainSizeShieldsNumber(const double grainSizeShieldsNumber);

    /// Função de retorno do valor do declive
    /// \return Retorna o valor da declive
    std::shared_ptr<continental::datamanagement::Raster<double> > getSlope() const;

    /// Método de definição do valor do declive
    /// \param slope Valor do declive
    void setSlope(const std::shared_ptr<continental::datamanagement::Raster<double> > slope);

    /// Função de retorno do valor d50
    /// \return Retorna o valor d50
    std::shared_ptr<continental::datamanagement::Raster<double> > getD50() const;

    /// M<étodo de definição do valor d50
    /// \param d50 Valor d50
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
