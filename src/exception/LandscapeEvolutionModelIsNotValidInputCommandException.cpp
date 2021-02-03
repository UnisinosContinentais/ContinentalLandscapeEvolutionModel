#include "continental/landscapeevolutionmodel/exception/LandscapeEvolutionModelIsNotValidInputCommandException.h"

using namespace continental::landscapeevolutionmodel::exception;

LandscapeEvolutionModelIsNotValidInputCommandException::LandscapeEvolutionModelIsNotValidInputCommandException():
std::runtime_error("Dados inválidos para realizar o processo do lem!")
{

}
