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
    HydroToolsAlgorithmService(std::shared_ptr<continental::datamanagement::Raster<double>> initialGrid,
                               std::shared_ptr<continental::landscapeevolutionmodel::dto::LandscapeEvolutionModelInput> inputParameters,
                               std::shared_ptr<continental::datamanagement::Raster<short>> underwaterSeparatedGrid);

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

    const std::shared_ptr<std::vector<std::shared_ptr<hydrotools::service::CellWatershed>>> & getCellsExhilarating() const;

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
};

}
}
}
#endif // CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_HYDROTOOLSALGORITHM_H
