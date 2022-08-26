/*
* Developed by UNISINOS
* author: Dante Vinicius Eloy Barbosa
* email: DANTEBARBOSA@unisinos.br
* date: March, 2020
*/
#ifndef CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_HDIFUSIONALGORITHMSERVICE_H
#define CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_HDIFUSIONALGORITHMSERVICE_H

#include <memory>
#include <QString>
#include <vector>
#include <continental/datamanagement/Raster.h>
#include "continental/landscapeevolutionmodel/export.h"

namespace continental {
namespace landscapeevolutionmodel {
namespace service {

/**
 * @brief Esta classe resolve a equação da difusão transiente implícita.
 */
class CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_EXPORT_DECL DifusionAlgorithmService
{

public:
    /// Construtor.
    DifusionAlgorithmService();

    /// Método responsável pelo algoritmo de defusão
    /// \param initialGrid Objeto do tipo Raster<float> com a Superfície Inicial.
    /// \param getDiffusivity Valor do tipo double com o parametro de difusividade.
    /// \param deltaT Valor do tipo size_t com o passo de tempo de anos.
    DifusionAlgorithmService(std::shared_ptr<continental::datamanagement::Raster<double>> initialGrid, double getDiffusivity, size_t deltaT);
	
    /// Função que executa o algoritmo de alocação da topográfia.
    void allocateTopography();

    /// Função que executa o algoritmo de difusividade.
    void execute();

    /// Função que executa o algoritmo de difusividade.
    /// \param eastBoundaryFactor Valor do fator da banda Leste
    /// \param westBoundaryFactor Valor do fator da banda Oeste
    /// \param southBoundaryFactor Valor do fator da banda Sul
    /// \param northBoundaryFactor Valor do fator da banda Norte
    void executeWithVariableBoundary(size_t eastBoundaryFactor, size_t westBoundaryFactor, size_t southBoundaryFactor, size_t northBoundaryFactor);

    /// Função Getter referente a difusidade
    /// \return Retorna o valor da difusidade
    double getDiffusivity() const;

private:
    /// Membros.
    /// Observações: m_deltaT em ano, deltaX e Y em metros, difusidade em m²/ano
    /// e m_T referente a matriz de topografia
    std::shared_ptr<continental::datamanagement::Raster<double>> m_initialGrid;
    size_t m_numberOfCols = 0;
    size_t m_numberOfRows = 0;
    size_t m_deltaT = 0;
    size_t m_numberOfIterations = 0;
    double m_deltaX = 0.0;
    double m_deltaY = 0.0;
    double m_diffusivity = 0.0;
    double m_upLift = 0.0;
    std::vector<std::vector<std::vector<std::vector<double>>>> m_T;


};

}
}
}

#endif // CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_HDIFUSIONALGORITHMSERVICE_H
