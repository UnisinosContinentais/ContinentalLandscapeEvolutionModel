/*
* Developed by UNISINOS - Projeto Continentais and Petrobras
* author: Luiz Felipe Bertoldi de Oliveira
* email: lbertoldio@unisinos.br
* date: March, 2020
*/

#ifndef CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_DOMAIN_HEURISTICSINKREMOVALPROCESSINGMODE_H
#define CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_DOMAIN_HEURISTICSINKREMOVALPROCESSINGMODE_H

namespace continental {
namespace landscapeevolutionmodel {
namespace domain {
	
enum class HeuristicSinkRemovalProcessingMode
{
    // Utiliza o modo Modified Heuristic Search
    MHS = 0,
    // Utiliza o modo Heuristic Search
    HS = 1,
    // Utiliza o modo PFS
    PFS = 2
};

}
}
}

#endif // CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_DOMAIN_HEURISTICSINKREMOVALPROCESSINGMODE_H
