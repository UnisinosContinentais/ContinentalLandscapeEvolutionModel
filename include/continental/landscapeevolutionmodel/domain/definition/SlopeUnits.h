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


enum class SlopeUnits
{
    tangent = 1,
    percent = 2,
    degree = 3,
    radians = 4
};

}
}
}
}

#endif // CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_DOMAIN_DEFINITION_SLOPEUNITS_H
