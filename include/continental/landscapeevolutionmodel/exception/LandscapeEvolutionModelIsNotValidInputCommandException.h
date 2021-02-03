/*
* Developed by UNISINOS - Projeto Continentais and Petrobras
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: March, 2020
*/

#ifndef CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_EXCEPTION_LANDSCAPEEVOLUTIONMODEL_IS_NOT_VALID_DATA_H
#define CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_EXCEPTION_LANDSCAPEEVOLUTIONMODEL_IS_NOT_VALID_DATA_H

#include <cstddef>
#include <QString>
#include <iostream>
#include <exception>

namespace continental {
namespace landscapeevolutionmodel {
namespace exception {

/**
 * @brief Classe de excessão de validade dos parâmetros de entrada do processo do Modelo de Evolução Topográfica(LEM).
 * Esta classe contém funções para definir as exceções dos parâmetros de entrada do processo Landscape Evolution Model.
 */
class LandscapeEvolutionModelIsNotValidInputCommandException : virtual public std::runtime_error
{
public:
    /// Construtor.
    explicit LandscapeEvolutionModelIsNotValidInputCommandException();

    /// Destrutor.
    virtual ~LandscapeEvolutionModelIsNotValidInputCommandException() noexcept = default;
};

}
}
}

#endif // CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_EXCEPTION_LANDSCAPEEVOLUTIONMODEL_IS_NOT_VALID_DATA_H
