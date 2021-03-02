#include "continental/landscapeevolutionmodel/exception/LandscapeEvolutionModelIsNotValidInputCommandException.h"

namespace continental {
namespace landscapeevolutionmodel {
namespace exception {

LandscapeEvolutionModelIsNotValidInputCommandException::LandscapeEvolutionModelIsNotValidInputCommandException():
std::runtime_error("Dados inválidos para realizar o processo do lem!")
{

}

} // namespace exception
} // namespace landscapeevolutionmodel
} // namespace continental
