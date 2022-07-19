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
    //construtor
    SedimentaryInputContent();

    const unsigned long long & getPositionI() const;
    void setPositionI(const unsigned long long &positionI);

    const unsigned long long & getPositionJ() const;
    void setPositionJ(const unsigned long long &positionJ);

    const double & getSedimentaryInputContent() const;
    void setSedimentaryInputContent(const double &sedimentaryInputContent);

private:

    //membros da classe
    unsigned long long m_positionI = 0;
    unsigned long long m_positionJ = 0;
    double m_sedimentaryInputContent = 0.0;
};

}
}
}


#endif // CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_DOMAIN_SEDIMENTARYINPUTCONTENT_H
