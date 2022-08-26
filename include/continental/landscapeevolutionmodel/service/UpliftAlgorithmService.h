/*
* Developed by UNISINOS
* author: Dante Vinicius Eloy Barbosa
* email: DANTEBARBOSA@unisinos.br
* date: March, 2020
*/
#ifndef CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_SERVICE_UpliftAlgorithmService_H
#define CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_SERVICE_UpliftAlgorithmService_H

#include "continental/landscapeevolutionmodel/export.h"
#include <continental/datamanagement/Raster.h>
#include <QString>
#include <memory>
#include <cmath>
#include <vector>

namespace continental {
namespace landscapeevolutionmodel {
namespace service {

/**
 * @brief Esta classe resolve a equação da erosão transiente explícita.
 */
class CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_EXPORT_DECL UpliftAlgorithmService
{
public:
    /// Construtor.
    UpliftAlgorithmService();

    /// Método responsável pelo serviço do algoritmo de erodibilidade
    /// \param initialGrid Valor inicial do grid
    /// \param upliftRate Valor da taxa de soerguimento
    /// \param timeStep Valor do passo de tempo
    /// \param numberOfTimeSteps Valor do número dos passos de tempo
    UpliftAlgorithmService(std::shared_ptr<datamanagement::Raster<double>> initialGrid,
                           std::shared_ptr<datamanagement::Raster<double> > upliftRate,
                           double timeStep,
                           size_t numberOfTimeSteps);


    /// Função de retorno do grid inicial
    /// \return Retorna p valor do grid inicial
    std::shared_ptr<datamanagement::Raster<double> > getInitialGrid() const;

    /// Função de definição do grid inicial
    /// \param initialGrid Valor do grid inicial
    void setInitialGrid(const std::shared_ptr<datamanagement::Raster<double> > &initialGrid);

    /// Função de retorno do passo de tempo
    /// \return Retona o valor do passo de tempo
    double getTimeStep() const;

    /// Função de definição do passo de tempo
    /// \param timeStep Valor do passo de tempo
    void setTimeStep(double timeStep);

    /// Função de retorno da taxa de erodibilidade
    /// \return Retorna o valor da taxa de erobilidade
    std::shared_ptr<datamanagement::Raster<double>> getUpliftRate() const;

    /// Função de definição da taxa de erodibilidade
    /// \param upliftRate Valor da taxa de erodibilidade
    void setUpliftRate(const std::shared_ptr<datamanagement::Raster<double>> &upliftRate);

    /// Função de retorno do número do passo de tempo
    /// \return Retorna o valor do número do passo de tempo
    size_t getNumberOfTimeSteps() const;

    /// Função de definição do número do passo de tempo
    /// \param numberOfTimeSteps Valor do número do passo de tempo
    void setNumberOfTimeSteps(const size_t &numberOfTimeSteps);

    /// Construtor
    void applyUplift();

    /// Método responsável pelo valor total da erodibilidade
    /// \return Retorna o valor total da erodibilidade
    std::shared_ptr<datamanagement::Raster<double>> totalUplift();

private:
    ///Membros
    std::shared_ptr<datamanagement::Raster<double>> m_initialGrid;
    std::shared_ptr<datamanagement::Raster<double>> m_upliftRate;
    double m_timeStep = 0;
    size_t m_numberOfTimeSteps = 0;

};

}
}
}
#endif // CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_SERVICE_UpliftAlgorithmServiceq_H

