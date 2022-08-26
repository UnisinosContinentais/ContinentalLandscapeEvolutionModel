/*
* Developed by UNISINOS - Projeto Continentais and Petrobras
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: March, 2020
*/
#ifndef CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_HYDROTOOLSALGORITHM_H
#define CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_HYDROTOOLSALGORITHM_H

#include <memory>
#include <QString>
#include <vector>
#include <continental/datamanagement/Raster.h>
#include <continental/landscapeevolutionmodel/dto/LandscapeEvolutionModelInput.h>
#include "continental/landscapeevolutionmodel/export.h"

namespace continental {
namespace hydrotools {
namespace service {
    class CellWatershed;
}
}
namespace landscapeevolutionmodel {
namespace service {
/**
 * @brief Classe de extração de rede de Drenagem.
 */
class CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_EXPORT_DECL HydroToolsAlgorithmService
{
public:
    /// Construtor.
    HydroToolsAlgorithmService();

    /// Construtor.
    /// @param initialGrid Objeto do tipo Raster<float> com a Superfície Inicial.
    /// @param inputParameters Objeto do tipo LandscapeEvolutionModelInput com os parametros de entrada para realização do processamento.
    /// @param underwaterSeparatedGrid Valor do grid que separa os valores do nivel aquoso
    /// @param initialFlowDirection Valor inicial da direção do fluxo
    /// @param initialFlowAccumulation Valor inicial do fluxo acumulado
    HydroToolsAlgorithmService(std::shared_ptr<continental::datamanagement::Raster<double>> initialGrid,
                               std::shared_ptr<continental::landscapeevolutionmodel::dto::LandscapeEvolutionModelInput> inputParameters,
                               std::shared_ptr<continental::datamanagement::Raster<short>> underwaterSeparatedGrid,
                               std::shared_ptr<continental::datamanagement::Raster<short>> initialFlowDirection,
                               std::shared_ptr<continental::datamanagement::Raster<int>> initialFlowAccumulation);

    /// Função que executa a preparação da topografia para execução do processo principal.
    void prepareDem();

    /// Função que executa o hydrotools.
    void execute();

    /// Executa o cálculo que identifica as bacias
    void executeWaterShed();

    /// Função de retorno dos valores de Direção do Fluxo.
    /// @return Objeto do tipo Raster<short>.
    const std::shared_ptr<continental::datamanagement::Raster<short>> getFlowDirection() const;

    /// Função de retorno dos valores de Acumulação do Fluxo.
    /// @return Objeto do tipo Raster<short>.
    const std::shared_ptr<continental::datamanagement::Raster<int>> getFlowAccumulation() const;

    /// Função de retorno dos valores de Definição dos Rios.
    /// @return Objeto do tipo Raster<short>.
	const std::shared_ptr<continental::datamanagement::Raster<short>> getStreamDefinition() const;

    /// Função de retorno dos valores de Segmentação dos Rios.
    /// @return Objeto do tipo Raster<short>.
	const std::shared_ptr<continental::datamanagement::Raster<short>> getStreamSegmentation() const;

    /// Função de retorno dos valores de delimitação da bacia hidrográfica.
    /// @return Objeto do tipo Raster<short>.
    const std::shared_ptr<continental::datamanagement::Raster<short>> & getWaterShed() const;

    /// Função de retorno dos exutórios
    /// \return Retorna o valor dos exutórios
    const std::shared_ptr<std::vector<std::shared_ptr<hydrotools::service::CellWatershed>>> & getCellsExhilarating() const;

    /// Função de retorno do grid separador do meio aquoso
    /// \return Retorna o valor do grid separado do meio aquoso
    std::shared_ptr<continental::datamanagement::Raster<short> > getUnderwaterSeparatedGrid() const;

private:
    /// Membros.
    std::shared_ptr<continental::datamanagement::Raster<double>> m_initialGrid;
    std::shared_ptr<continental::datamanagement::Raster<short>> m_flowDirection;
    std::shared_ptr<continental::datamanagement::Raster<int>> m_flowAccumulation;
	std::shared_ptr<continental::datamanagement::Raster<short>> m_streamDefinition;
	std::shared_ptr<continental::datamanagement::Raster<short>> m_streamSegmentation;
    std::shared_ptr<continental::datamanagement::Raster<short>> m_waterShed;
    std::shared_ptr<std::vector<std::shared_ptr<hydrotools::service::CellWatershed>>> m_cellExhilarating;
    std::shared_ptr<continental::landscapeevolutionmodel::dto::LandscapeEvolutionModelInput> m_inputParameters;
    std::shared_ptr<continental::datamanagement::Raster<short>> m_underwaterSeparatedGrid;
    std::shared_ptr<continental::datamanagement::Raster<short>> m_initialFlowDirection = nullptr;
    std::shared_ptr<continental::datamanagement::Raster<int>> m_initialFlowAccumulation = nullptr;
};

}
}
}
#endif // CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_HYDROTOOLSALGORITHM_H
