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

enum class StreamDefinitionThresholdType
{
    NumberOfCells = 0,
    PercentualOfMaximumCells = 1,
    Area = 2 //km²
};

} // namespace domain
} // namespace landscapeevolutionmodel
} // namespace continental

#endif // CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_DOMAIN_STREAMDEFINITIONTHRESHOLDTYPE_H
