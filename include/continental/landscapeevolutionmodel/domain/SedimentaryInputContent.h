/*
* Developed by UNISINOS - Projeto Continentais and Petrobras
* author: Thiago Soares Seidel
* email: thiagosseidel@unisinos.br
* date: Feb, 2021
*/
#ifndef CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_DOMAIN_SEDIMENTARYINPUTCONTENT_H
#define CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_DOMAIN_SEDIMENTARYINPUTCONTENT_H

#include <QString>

namespace continental {
namespace landscapeevolutionmodel {
namespace domain {

/**
 * @brief Classe de definição dos dados de sedimentação.
 */
class SedimentaryInputContent
{

public:

    /// Construtor
    SedimentaryInputContent();

    /// Função Getter referente a posição I
    /// \return Retorna o valor da posição I
    const unsigned long long & getPositionI() const;

    /// Função Setter referente a posição I
    /// \param positionI Valor da posição I
    void setPositionI(const unsigned long long &positionI);

    /// Função Getter referente a posição J
    /// \return Retorna o valor da posição J
    const unsigned long long & getPositionJ() const;

    /// Função Setter referente a posição J
    /// \param positionJ Valor da posição J
    void setPositionJ(const unsigned long long &positionJ);

    /// Função Getter referente aos conteúdos de entrada da Sedimentação
    /// \return Retorna o objeto com os conteúdos de entrada da Sedimentação
    const double & getSedimentaryInputContent() const;

    /// Função Setter referente aos conteúdos de entrada da Sedimentação
    /// \param sedimentaryInputContent
    void setSedimentaryInputContent(const double &sedimentaryInputContent);

private:

    ///Membros
    unsigned long long m_positionI = 0;
    unsigned long long m_positionJ = 0;
    double m_sedimentaryInputContent = 0.0;
};

}
}
}


#endif // CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_DOMAIN_SEDIMENTARYINPUTCONTENT_H
