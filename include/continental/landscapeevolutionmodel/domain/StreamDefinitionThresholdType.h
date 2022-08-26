/*
* Developed by UNISINOS - Projeto Continentais and Petrobras
* author: Luiz Felipe Bertoldi de Oliveira
* email: lbertoldio@unisinos.br
* date: March, 2020
*/

#ifndef CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_DOMAIN_STREAMDEFINITIONTHRESHOLDTYPE_H
#define CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_DOMAIN_STREAMDEFINITIONTHRESHOLDTYPE_H

namespace continental {
namespace landscapeevolutionmodel {
namespace domain {

///
/// \brief The StreamDefinitionThresholdType enum Classe emun referente ao limiar
/// dos tipos do definição dos riachos
///
enum class StreamDefinitionThresholdType
{

    ///Número de celulas
    NumberOfCells = 0,

    ///Percentual maximo de celulas
    PercentualOfMaximumCells = 1,

    /// Area em Km²
    Area = 2


};

} // namespace domain
} // namespace landscapeevolutionmodel
} // namespace continental

#endif // CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_DOMAIN_STREAMDEFINITIONTHRESHOLDTYPE_H
