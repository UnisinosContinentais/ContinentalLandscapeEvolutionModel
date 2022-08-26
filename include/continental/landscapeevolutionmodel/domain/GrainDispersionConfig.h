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


///
/// \brief The GrainDispersionConfig class Classe da Sedimentação responsável pela
/// configuração dos parâmetros de dispersão do grão. Indice e relação de escoamento,
/// indices C e K, densidade do fluido e sedimento e número de shields
///
class CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_EXPORT_DECL GrainDispersionConfig
{
public:

    ///  Método Getter referente ao parametro do indice de escoamento K
    /// \return Retorna o valor do indice de escoamento K
    double getDischargeKParameter() const;

    /// Método Setter referente ao indice de escoamento K
    /// \param dischargeKParameter Valor do indice de escoamento K
    void setDischargeKParameter(double dischargeKParameter);

    /// Método Getter referente a relação de escoamento E
    /// \return Retorna o valor da relação de escoamento E
    double getDischargeEParameter() const;

    /// Método Setter referente a relação de escoamento E
    /// \param dischargeEParameter Valor do parâmetro dischargeE
    void setDischargeEParameter(double dischargeEParameter);

    /// Método Getter referente ao indice C da geometria do canal
    /// \return Retorna o valor o indice C da geometria do canal
    double getChannelDepthCParameter() const;

    /// Método Setter referente ao indice C da geometria do canal
    /// \param channelDepthCParameter Valor do indice C da geometria do canal
    void setChannelDepthCParameter(double channelDepthCParameter);

    /// Método Getter referente ao indice F da geometria do canal
    /// \return Retorna o valor o indice F da geometria do canal
    double getChannelDepthFParameter() const;

    /// Método Setter referente ao indice F da geometria do canal
    /// \param channelDepthFParameter Valor o indice F da geometria do canal
    void setChannelDepthFParameter(double channelDepthFParameter);

    /// Método Getter referente desidade do fluido em Kg/m³
    /// \return Retorna o valor da desidade do fluido
    double getGrainSizeWaterDensity() const;

    /// Método Setter referente desidade do fluido em Kg/m³
    /// \param grainSizeWaterDensity Valor da desidade do fluido
    void setGrainSizeWaterDensity(double grainSizeWaterDensity);

    /// Método Getter referente a densidade do sedimento em Kg/m³
    /// \return Retorna o valor da densidade do sedimento
    double getGrainSizeSedimentDensity() const;

    /// Método Setter referente a densidade do sedimento em Kg/m³
    /// \param grainSizeSedimentDensity Valor da densidade do sedimento
    void setGrainSizeSedimentDensity(double grainSizeSedimentDensity);

    /// Método Getter referente ao número de shields com valor adimensional
    /// \return Retorna o número de shields
    double getGrainSizeShieldsNumber() const;

    /// Método Setter referente ao número de shields com valor adimensional
    /// \param grainSizeShieldsNumber Valor do número de shields
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
