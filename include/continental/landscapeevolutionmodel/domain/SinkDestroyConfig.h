/*
* Developed by UNISINOS - Projeto Continentais and Petrobras
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: March, 2020
*/
#ifndef CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_DOMAIN_SINKDESTROYCONFIG_1H
#define CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_DOMAIN_SINKDESTROYCONFIG_1H

#include "HeuristicSinkRemovalProcessingMode.h"
#include "../export.h"

namespace continental {
namespace landscapeevolutionmodel {
namespace domain {

/**
 * @brief Classe de definição da configuração processo de Sink Destroy.
 * Esta classe contém funções para configurar o processo Sink Destroy do Continental Hydro Tools.
 */
class CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_EXPORT_DECL  SinkDestroyConfig
{
public:
    /// Função de retorno do valor do número máximo de células abertas.
    /// @return Retorna o valor do número máximo de células abertas.
    size_t getMaxOpenList() const;

    /// Função de definição do valor do número máximo de células abertas.
    /// @param value Valor do número máximo de células abertas.
    void setMaxOpenList(size_t value);

    /// Função de retorno do valor do número máximo de células fechadas.
    /// @return Retorna o valor do número máximo de células fechadas.
    size_t getMaxClosedList() const;

    /// Função de definição do valor do número máximo de células fechadas.
    /// @param value Valor do número máximo de células fechadas.
    void setMaxClosedList(size_t value);

    /// Função de retorno do valor do peso da função.
    /// @return Retorna o valor do peso da função.
    double getCostFunctionWeight() const;

    /// Função de definição do valor do peso da função.
    /// @param value Valor do peso da função.
    void setCostFunctionWeight(double value);

    /// Função de retorno do enum do algoritmo de processo.
    /// @return Retorna o enum do algoritmo de processo.
    HeuristicSinkRemovalProcessingMode getProcessingAlgorithm() const;

    /// Função de retorno do nome do algoritmo de processo.
    /// @return Retorna o nome do algoritmo de processo.
    QString getProcessingAlgorithmName() const;

    /// Função de definição do enum do algoritmo de processo.
    /// @param name Enum do algoritmo de processo.
    void setProcessingAlgorithm(HeuristicSinkRemovalProcessingMode name);

    /// Função de retorno do valor da versão.
    /// @return Valor da versão.
    unsigned short getVersion() const;

    /// Função de definição do valor da versão.
    /// @param version Valor da versão.
    void setVersion(unsigned short version);

private:
    /// Membros.
    size_t m_maxOpenList = 0;
    size_t m_maxClosedList = 0;
    double m_costFunctionWeight = 0;
    HeuristicSinkRemovalProcessingMode m_processingAlgorithm;
    unsigned short m_version = 0;
};

} //domain
} //landscapeevolutionmodel
} //continental

#endif //CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_DOMAIN_SINKDESTROYCONFIG_1H
