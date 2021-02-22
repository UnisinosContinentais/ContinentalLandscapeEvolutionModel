/*
* Developed by UNISINOS - Projeto Continentais and Petrobras
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: March, 2020
*/
#ifndef CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_DOMAIN_STREAMDEFINITIONCONFIG_H
#define CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_DOMAIN_STREAMDEFINITIONCONFIG_H

#include "../export.h"
#include "StreamDefinitionThresholdType.h"

namespace continental {
namespace landscapeevolutionmodel {
namespace domain {

/**
 * @brief Classe de definição da configuração processo de Stream Definition.
 * Esta classe contém funções para configurar o processo Stream Definition do Continental Hydro Tools.
 */
class CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_EXPORT_DECL StreamDefinitionConfig
{
public:
    /// Função de retorno do valor da área mínima.
    /// @return Retorna o valor da área mínima.
    double getThresoldValue() const;

    /// Função de definição do valor da área mínima.
    /// @param thresoldValue Retorna o valor da área mínima.
    void setThresoldValue(double thresoldValue);

    /// Função de retorno do enum do tipo de algoritmo.
    /// @return Retorna o enum do tipo de algoritmo.
    StreamDefinitionThresholdType getThresoldType() const;

    /// Função de retorno do nome do tipo de algoritmo.
    /// @return Retorna o nome do tipo de algoritmo.
    QString getThresoldTypeName() const;

    /// Função de definição do enum do tipo de algoritmo.
    /// @param thresoldType Enum do tipo de algoritmo.
    void setThresoldType(QString thresoldTypeId);

private:
    /// Membros.
    double m_thresoldValue = 0.0f; //Acho que o usuário deve poderalterar esse valor
    StreamDefinitionThresholdType m_thresoldType;
};

} //fluvialsystem
} //domain
} //continental

#endif // CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_DOMAIN_STREAMDEFINITIONCONFIG_H
