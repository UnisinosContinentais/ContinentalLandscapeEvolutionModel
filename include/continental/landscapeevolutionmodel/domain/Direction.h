/*
* Developed by UNISINOS - Projeto Continentais and Petrobras
* author: Luiz Felipe Bertoldi de Oliveira
* email: lbertoldio@unisinos.br
* date: October, 2020
*/
#ifndef CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_DOMAIN_DIRECTION_H
#define CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_DOMAIN_DIRECTION_H

#include <vector>
#include <memory>

namespace continental {
namespace landscapeevolutionmodel {
namespace domain {

///
/// \brief The Direction class Classe responsável pela direção em função da linha e coluna
///
class Direction
{
public:

    /// Função Direction responsável pela linha e coluna
    /// \param row parâmetro referente a linha
    /// \param col parâmetro referente a coluna
    Direction(size_t row, size_t col);

    /// getRow Getter referente a linha
    /// \return Retorna o valor da linha
    size_t getRow() const;

    /// getCol Getter referente a coluna
    /// \return Retorna o valor da coluna
    size_t getCol() const;

    /// Vector direção utilizado no processo que percorre cada campo da matriz
    /// e realiza o ajuste em relação aos vizinhos. Conforme artigo do HydroTools
    std::vector<Direction> donors;

private:
    /// Membros
    size_t m_row = 0;
    size_t m_col = 0;
};

} // namespace domain
} // namespace landscapeevolutionmodel
} // namespace continental

#endif // CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_DOMAIN_DIRECTION_H
