/*
* Developed by UNISINOS - Projeto Continentais and Petrobras
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: March, 2020
*/
#ifndef CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_DOMAIN_SINKDESTROYCONFIG_H
#define CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_DOMAIN_SINKDESTROYCONFIG_H

#include "../export.h"
#include "../domain/EnumDrainageNetworkLimit.h"

namespace continental {
namespace landscapeevolutionmodel {
namespace domain {

/**
 * @brief Classe de definição da configuração processo do Modelo de Evolução Topográfica(LEM).
 * Esta classe contém funções para configurar o processo Landscape Evolution Model.
 */
class CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_EXPORT_DECL SimulationLandscapeEvolutionModelConfig
{
public:
    /// Função de retorno do valor da versão.
    /// @return Valor da versão.
    unsigned short getVersion() const;

    /// Função de definição do valor da versão.
    /// @param version Valor da versão.
    void setVersion(unsigned short version);

    /// Função de retorno do valor da Erodibilidade.
    /// @return Valor da Erodibilidade.
    double getErodibility() const;

    /// Função de definição do valor da Erodibilidade.
    /// @param erodibility valor da Erodibilidade.
    void setErodibility(const double &erodibility);

    /// Função de retorno do valor da Difusividade.
    /// @return Valor da Difusividade.
    double getDiffusivity() const;

    /// Função de definição do valor da Difusividade.
    /// @param diffusion valor da Difusividade.
    void setDiffusivity(const double &diffusivity);

    /// Função de retorno do índice de concavidade.
    /// @return Índice de concavidade.
    double getConcavityIndex() const;

    /// Função de definição do índice de concavidade.
    /// @param concavityIndex Índice de concavidade.
    void setConcavityIndex(const double &concavityIndex);

    /// Função de retorno do valor do Parâmetro N.
    /// @return Valor da parâmetro N.
    double getValueN() const;

    /// Função de definição do valor do Parâmetro N.
    /// @param valueN Valor do Parâmetro N.
    void setValueN(const double &valueN);

    size_t getEastBoundaryFactor() const;
    void setEastBoundaryFactor(const size_t &eastBoundaryFactor);

    size_t getWestBoundaryFactor() const;
    void setWestBoundaryFactor(const size_t &westBoundaryFactor);

    size_t getSouthBoundaryFactor() const;
    void setSouthBoundaryFactor(const size_t &southBoundaryFactor);

    size_t getNorthBoundaryFactor() const;
    void setNorthBoundaryFactor(const size_t &northBoundaryFactor);

    size_t getDrainagesLength() const;
    void setDrainagesLength(const size_t &drainagesLength);

    double getDimensionLessPrecipitationRate() const;
    void setDimensionLessPrecipitationRate(double precipitationRate);

    double getDimensionLessDepositionCoeficient() const;
    void setDimensionLessDepositionCoeficient(double depositionCoeficient);

    void useOnlyMainDrainageNetwork();

    void useDrainageNetworkAmountLimit(size_t amountLimit);

    void useDrainageNetworkPercentLimit(double percentLimit);

    domain::EnumDrainageNetworkLimit getDrainageNetworkTypeLimit() const;

    size_t getDrainageNetworkAmountLimit() const;

    double getDrainageNetworkPercentLimit() const;

private:
    /// Membros.
    double m_erodibility = 0.0;
    double m_diffusivity = 0.0;
    double m_concavityIndex = 0.0;
    double m_valueN = 0.0;
    double m_dimensionLessPrecipitationRate = 0.0;
    double m_dimensionLessDepositionCoeficient = 0.0;
    domain::EnumDrainageNetworkLimit m_drainageNetworkTypeLimit = domain::EnumDrainageNetworkLimit::OnlyMain;
    size_t m_drainageNetworkAmountLimit = 0;
    double m_drainageNetworkPercentLimit = 0.0;

    size_t m_eastBoundaryFactor = 0;
    size_t m_westBoundaryFactor = 0;
    size_t m_southBoundaryFactor = 0;
    size_t m_northBoundaryFactor= 0;

    unsigned short m_version = 0;
};

} //domain
} //landscapeevolutionmodel
} //continental

#endif //CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_DOMAIN_SINKDESTROYCONFIG_H
