/*
* Developed by UNISINOS - Projeto Continentais and Petrobras
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: November, 2020
*/
#ifndef CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_DOMAIN_GRAINDISPERSIONCONFIG_H
#define CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_DOMAIN_GRAINDISPERSIONCONFIG_H

#include "../export.h"

namespace continental {
namespace landscapeevolutionmodel {
namespace domain {


class CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_EXPORT_DECL GrainDispersionConfig
{
public:
    double getDischargeKParameter() const;
    void setDischargeKParameter(double dischargeKParameter);

    double getDischargeEParameter() const;
    void setDischargeEParameter(double dischargeEParameter);

    double getChannelDepthCParameter() const;
    void setChannelDepthCParameter(double channelDepthCParameter);

    double getChannelDepthFParameter() const;
    void setChannelDepthFParameter(double channelDepthFParameter);

    double getGrainSizeWaterDensity() const;
    void setGrainSizeWaterDensity(double grainSizeWaterDensity);

    double getGrainSizeSedimentDensity() const;
    void setGrainSizeSedimentDensity(double grainSizeSedimentDensity);

    double getGrainSizeShieldsNumber() const;
    void setGrainSizeShieldsNumber(double grainSizeShieldsNumber);

private:
    /// Membros.
    double m_dischargeKParameter;
    double m_dischargeEParameter;
    double m_channelDepthCParameter;
    double m_channelDepthFParameter;
    double m_grainSizeWaterDensity;
    double m_grainSizeSedimentDensity;
    double m_grainSizeShieldsNumber;
};

} //domain
} //landscapeevolutionmodel
} //continental

#endif //CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_DOMAIN_SINKDESTROYCONFIG_H
