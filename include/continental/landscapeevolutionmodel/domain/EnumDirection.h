#ifndef CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_DOMAIN_ENUMDIRECTION_H
#define CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_DOMAIN_ENUMDIRECTION_H

namespace continental {
namespace landscapeevolutionmodel {
namespace domain {

///
/// \brief The EnumDirection enum Classe com a composição da codificação D8
/// do método utilizado pelo Hydrotools
///
enum EnumDirection
{
    Right = 1,
    DownRight = 2,
    Down = 4,
    DownLeft = 8,
    Left = 16,
    UpLeft = 32,
    Up = 64,
    UpRight = 128
};

} // namespace domain
} // namespace landscapeevolutionmodel
} // namespace continental

#endif // CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_DOMAIN_ENUMDIRECTION_H
