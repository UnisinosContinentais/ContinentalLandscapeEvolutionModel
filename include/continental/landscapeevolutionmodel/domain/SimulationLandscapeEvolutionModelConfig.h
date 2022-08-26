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
#include <vector>

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
    /// @param diffusivity valor da Difusividade.
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

    /// Função de retorno do valor do fator da banda Leste
    /// \return Retorna o valor do fator da banda Leste
    size_t getEastBoundaryFactor() const;

    /// Função de definição do valor do fator da banda Leste
    /// \param eastBoundaryFactor Valor do fator da banda Leste
    void setEastBoundaryFactor(const size_t &eastBoundaryFactor);

    /// Função de retorno do valor do fator da banda Oeste
    /// \return Retorna o valor do fator da banda Oeste
    size_t getWestBoundaryFactor() const;

    /// Função de definição do valor do fator da banda Oeste
    /// \param westBoundaryFactor Valor do fator da banda Oeste
    void setWestBoundaryFactor(const size_t &westBoundaryFactor);

    /// Função de retorno do valor do fator da banda Sul
    /// \return Retorna o valor do fator da banda Sul
    size_t getSouthBoundaryFactor() const;

    /// Função de definição do valor do fator da banda Sul
    /// \param southBoundaryFactor Valor do fator da banda Sul
    void setSouthBoundaryFactor(const size_t &southBoundaryFactor);

    /// Função de retorno do valor do fator da banda Norte
    /// \return Retorna o valor do fator da banda Norte
    size_t getNorthBoundaryFactor() const;

    /// Função de definição do valor do fator da banda Norte
    /// \param northBoundaryFactor Valor do fator da banda Norte
    void setNorthBoundaryFactor(const size_t &northBoundaryFactor);

    /// Função de retorno da taxa de precipitação adimensional
    /// \return Retorna o valor da taxa de precipitação adimensional
    double getDimensionLessPrecipitationRate() const;

    /// Função de definição da taxa de precipitação adimensional
    /// \param precipitationRate Valor referente a taxa de precipitação adimencional
    void setDimensionLessPrecipitationRate(double precipitationRate);

    /// Função de retorno do coeficiente de precipitação adimensional
    /// \return Retorna o valor do coeficiente de precipitação adimensional
    double getDimensionLessDepositionCoeficient() const;

    /// Função de definição do coeficiente da precipitação adimensional
    /// \param depositionCoeficient Valor do coeficiente da precipitação adimensional
    void setDimensionLessDepositionCoeficient(double depositionCoeficient);

    /// Função do uso apenas da rede principal de prenagem
    void useOnlyMainDrainageNetwork();

    /// Função referente ao uso do limite da quantidade da rede de drenagem
    /// \param amountLimit Valor referente ao limite da quantidade do uso da rede de drenagem
    void useDrainageNetworkAmountLimit(size_t amountLimit);

    /// Função referente a porcentagem do uso da rede de drenagem
    /// \param percentLimit Valor referente a porcentagem do uso da rede de drenagem
    void useDrainageNetworkPercentLimit(double percentLimit);

    /// Função referente ao Enum de acordo com o limite da rede de drenagem
    /// \return Retorna o Enum do limite da rede de drenagem
    domain::EnumDrainageNetworkLimit getDrainageNetworkTypeLimit() const;

    /// Função de retorno da quantidade da rede de drenagem
    /// \return Retorna o valor em quantidade da rede de drenagem
    size_t getDrainageNetworkAmountLimit() const;

    /// Função de retorno da porcentagem da rede de drenagem
    /// \return Retorna o valor em porcentagem da rede de drenagem
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
