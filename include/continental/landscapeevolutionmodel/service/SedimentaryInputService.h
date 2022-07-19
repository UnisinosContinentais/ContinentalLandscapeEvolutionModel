/*
* Developed by UNISINOS
* author: Dante Vinicius Eloy Barbosa
* email: DANTEBARBOSA@unisinos.br
* date: March, 2020
*/
#ifndef CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_SERVICE_SEDIMENTARYINPUTSERVICE_H
#define CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_SERVICE_SEDIMENTARYINPUTSERVICE_H

#include "continental/landscapeevolutionmodel/export.h"
#include "continental/landscapeevolutionmodel/domain/EnumDrainageNetworkLimit.h"
#include "continental/landscapeevolutionmodel/domain/PositionMatrix.h"
#include "continental/landscapeevolutionmodel/domain/SedimentaryInputContent.h"
#include <continental/datamanagement/Raster.h>
#include <QString>
#include <memory>
#include <cmath>
#include <vector>

namespace continental {
namespace landscapeevolutionmodel {
namespace service {

/**
 * @brief Esta classe resolve a equação do aporte sedimentar.
 */
class CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_EXPORT_DECL SedimentaryInputService
{
public:
    /// Construtor.
    SedimentaryInputService();

    /// Função que executa o algoritimo de erodibilidade.
    void execute();

    //Atribui o Grid que possui as erosões e deposições
    void setOnlyErosionDepositionGrid(const std::shared_ptr<datamanagement::Raster<double>> & onlyErosionDepositionGrid);

    /// Função de definição dos valores de Direção do Fluxo.
    /// @param flowDirection Objeto do tipo Raster<short>.
    void setFlowDirection(const std::shared_ptr<datamanagement::Raster<short>> & flowDirection);

    /// Função de definição dos valores de Acumulação do Fluxo.
    /// @param flowAccumulation Objeto do tipo Raster<float>.
    void setFlowAccumulation(const std::shared_ptr<datamanagement::Raster<int>> & flowAccumulation);

    void useOnlyMainDrainageNetwork();

    void useDrainageNetworkAmountLimit(size_t amountLimit);

    void useDrainageNetworkPercentLimit(double percentLimit);

    int getFlowAccumulationLimit() const;

    void setFlowAccumulationLimit(int flowAccumulationLimit);

    const std::shared_ptr<std::vector<std::shared_ptr<domain::SedimentaryInputContent>>> & sedimentaryInputs() const;

private:

    /// Membros.
    size_t m_numberOfCols = 0;
    size_t m_numberOfRows = 0;
    double m_cellSize = 0.0;

    domain::EnumDrainageNetworkLimit m_drainageNetworkTypeLimit = domain::EnumDrainageNetworkLimit::OnlyMain;

    size_t m_drainageNetworkAmountLimit = 0;
    double m_drainageNetworkPercentLimit = 0.0;

    int m_flowAccumulationLimit = 0;

    std::shared_ptr<datamanagement::Raster<short>> m_flowDirection;
    std::shared_ptr<datamanagement::Raster<int>> m_flowAccumulation;
    std::shared_ptr<datamanagement::Raster<double>> m_onlyErosionDepositionGrid;    //Grid que possui as erosões e deposições

    std::shared_ptr<std::vector<std::shared_ptr<domain::SedimentaryInputContent>>> m_sedimentaryInputs;
};

}
}
}
#endif // CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_SERVICE_SEDIMENTARYINPUTSERVICE_H

