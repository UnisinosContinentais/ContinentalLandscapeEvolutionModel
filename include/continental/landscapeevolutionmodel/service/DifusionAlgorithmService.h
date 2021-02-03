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

    /// Construtor.
    /// @param initialGrid Objeto do tipo Raster<float> com a Superfície Inicial.
    /// @param simulateUtilTime Valor do tipo size_t com o tempo de simulacao em anos.
    /// @param diffusivity Valor do tipo double com o parametro de difusividade.
    /// @param deltaT Valor do tipo size_t com o passo de tempo de anos.
    DifusionAlgorithmService(std::shared_ptr<continental::datamanagement::Raster<float>> initialGrid, size_t simulateUtilTime, double diffusivity, size_t deltaT);
	
    /// Função que executa o algoritmo de alocação da topográfia.
    void allocateTopography();

    /// Função que executa o algoritmo de difusividade.
    void execute();

    /// Função que executa o algoritmo de difusividade.
    void executeWithVariableBoundary(size_t eastBoundaryFactor, size_t westBoundaryFactor, size_t southBoundaryFactor, size_t northBoundaryFactor);

private:
    /// Membros.
    std::shared_ptr<continental::datamanagement::Raster<float>> m_initialGrid;
    size_t m_numberOfCols = 0;
    size_t m_numberOfRows = 0;
    size_t m_deltaT = 0; // ano
    size_t m_numberOfIterations = 0;
    size_t m_simulateUltilTime = 0;
    double m_deltaX = 0.0; // m
    double m_deltaY = 0.0; // m
    double m_diffusivity = 0.0; // m*m/ano
    double m_upLift = 0.0;
    std::vector<std::vector<std::vector<std::vector<double>>>> m_T;  // matriz de topografia


};

}
}
}

#endif // CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_HDIFUSIONALGORITHMSERVICE_H
