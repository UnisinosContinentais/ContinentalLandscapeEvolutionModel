/*
* Developed by UNISINOS - Projeto Continentais and Petrobras
* author: Rudi C. C. Modena
* email: rmodena@unisinos.br
* date: October, 2020
*/
#ifndef CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_DOMAIN_DEFINITION_SLOPETYPES_H
#define CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_DOMAIN_DEFINITION_SLOPETYPES_H

#include "continental/landscapeevolutionmodel/export.h"

namespace continental {
namespace landscapeevolutionmodel {
namespace domain {
namespace definition {


enum class SlopeType
{
    slopeHorn1981 = 1,
    maxSlope = 2
};

}
}
}
}

#endif // CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_DOMAIN_DEFINITION_SLOPETYPES_H
