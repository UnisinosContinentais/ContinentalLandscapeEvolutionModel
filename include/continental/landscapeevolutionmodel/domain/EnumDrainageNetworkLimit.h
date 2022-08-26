#ifndef CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_DOMAIN_ENUMDRAINAGENETWORKLIMIT_H
#define CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_DOMAIN_ENUMDRAINAGENETWORKLIMIT_H

namespace continental {
namespace landscapeevolutionmodel {
namespace domain {

///
/// \brief The EnumDrainageNetworkLimit enum Enum responsável pela definição do
/// limite da rede de drenagem. Não definida (1), Apenas a rede principal (1),
/// porcentagem (2) e quantidade (3)
enum EnumDrainageNetworkLimit
{
    Undefined = 0,
    OnlyMain = 1,
    Percent = 2,
    Amount = 3
};

} // namespace domain
} // namespace landscapeevolutionmodel
} // namespace continental

#endif // CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_DOMAIN_ENUMDRAINAGENETWORKLIMIT_H
