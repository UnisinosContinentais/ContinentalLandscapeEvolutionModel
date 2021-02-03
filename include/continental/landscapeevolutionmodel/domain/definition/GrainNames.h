/*
* Developed by UNISINOS - Projeto Continentais and Petrobras
* author: Rudi C. C. Modena
* email: rmodena@unisinos.br
* date: October, 2020
*/
#ifndef CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_DOMAIN_DEFINITION_GRAINNAMES_H
#define CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_DOMAIN_DEFINITION_GRAINNAMES_H

#include "continental/landscapeevolutionmodel/export.h"

namespace continental {
namespace landscapeevolutionmodel {
namespace domain {
namespace definition {

enum class GrainNames
{
    CoarseSand,
    MediumSand,
    FineSand,
    Silt,
    Clay,
    IntertidalGrainstone,
    Grainstone,
    Packstone,
    Wackestone,
    Mudstone,
    UndefinedLithology,
    Stromatolite,
    StromatoliteWithMagnesianClay,
    StromatoliteWithSiliciclasticClay,
    Laminite,
    LaminiteWithMagnesianClay,
    LaminiteWithSiliciclasticClay,
    LaminiteWithOrganicMatter,
    Spherulitite,
    SpherulititeWithDolomite,
    SpherulititeWithMagnesianClay,
    SpherulititeWithSiliciclasticClay,
    PackstoneWithClayeyGrains,
    Anhydrite,
    Breccia,
    Dolomite,
    Lamite,
    Marlstone,
    Rudstone,
    Silexite,
    Salt,
    Gravel,
    N_LITHOLOGY_TYPES,
};

}
}
}
}

#endif // CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_DOMAIN_DEFINITION_GRAINNAMES_H
