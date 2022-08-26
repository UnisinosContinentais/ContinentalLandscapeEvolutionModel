/*
* Developed by UNISINOS - Projeto Continentais and Petrobras
* author: Rudi C. C. Modena
* email: rmodena@unisinos.br
* date: October, 2020
*/
#ifndef CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_DOMAIN_DEFINITION_SLOPEUNITS_H
#define CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_DOMAIN_DEFINITION_SLOPEUNITS_H

#include "continental/landscapeevolutionmodel/export.h"

namespace continental {
namespace landscapeevolutionmodel {
namespace domain {
namespace definition {

///
/// \brief The SlopeUnits enum Classe com o enum referente aos tipos de unidade
/// de declive
///
enum class SlopeUnits
{
    /// valor tangencial
    tangent = 1,
    /// valor percentil %
    percent = 2,
    /// valor do grau
    degree = 3,
    /// valor em radiano
    radians = 4
};

}
}
}
}

#endif // CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_DOMAIN_DEFINITION_SLOPEUNITS_H
