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
namespace hydrotools {
namespace service {
    class CellWatershed;
}
}

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

    /// Função que executa o algoritimo do aporte sedimentar.
    void execute();

    /// Função que atribui o Grid que possui apenas as erosões e deposições
    /// \param onlyErosionDepositionGrid Valor do Grid que possui apenas as erosões e deposições
    void setOnlyErosionDepositionGrid(const std::shared_ptr<datamanagement::Raster<double>> & onlyErosionDepositionGrid);

    /// Função de definição das bacias.
    /// @param waterShed Objeto do tipo Raster<short>.
    void setWaterShed(const std::shared_ptr<datamanagement::Raster<short>> & waterShed);

    /// Função de definição dos exutórios
    /// \param cellExhilarating Valor dos exutórios
    void setCellExhilarating(const std::shared_ptr<std::vector<std::shared_ptr<hydrotools::service::CellWatershed>>> & cellExhilarating);

    /// Função responsável pelos dados de entrada da sedimentação
    /// \return Retorna o valor da entrada da sedimentação
    const std::shared_ptr<std::vector<std::shared_ptr<domain::SedimentaryInputContent>>> & sedimentaryInputs() const;

    /// Função de definção do tempo de simulação
    /// \param simulateUntilTime Valor do tempo de simulação
    void setSimulateUntilTime(const double simulateUntilTime);


private:

    /// Membros.
    size_t m_numberOfCols = 0;
    size_t m_numberOfRows = 0;
    double m_cellSize = 0.0;
    double m_simulateUntilTime = 1.0;

    std::shared_ptr<std::vector<std::shared_ptr<domain::SedimentaryInputContent>>> m_sedimentaryInputs;

    std::shared_ptr<datamanagement::Raster<short>> m_waterShed; // Grid que possui as bacias
    std::shared_ptr<datamanagement::Raster<double>> m_onlyErosionDepositionGrid; // Grid que possui as erosões e deposições
    std::shared_ptr<std::vector<std::shared_ptr<hydrotools::service::CellWatershed>>> m_cellExhilarating; // Células com a posição dos exutórios

};

}
}
}
#endif // CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_SERVICE_SEDIMENTARYINPUTSERVICE_H

