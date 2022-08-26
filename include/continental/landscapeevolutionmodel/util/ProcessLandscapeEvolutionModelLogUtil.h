/*
* Developed by UNISINOS - Projeto Continentais and Petrobras
* author: Dante Vinícius Eloy Barbosa 
* email: dantebarbosa@unisinos.br
* date: August, 2021
*/
#ifndef CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_UTIL_ProcessLandscapeEvolutionModelLogUtil_H
#define CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_UTIL_ProcessLandscapeEvolutionModelLogUtil_H

#include <continental/datamanagement/Raster.h>
#include "continental/landscapeevolutionmodel/export.h"
#include <memory>


namespace continental
{
namespace landscapeevolutionmodel
{
namespace domain {
    class SimulationLandscapeEvolutionModelConfig;
    class GrainDispersionConfig;
}

namespace dto {
    class LandscapeEvolutionModelInput;

}

namespace util
{
/** @brief Classe de utilidades do projeto do StratBR.
 * Esta classe contém  funções de utilidades do projeto do StratBR.
 */
class CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_EXPORT_DECL ProcessLandscapeEvolutionModelLogUtil
{
public:

    /// Método de escrita dos Logs
    /// \param pathParameters   Valor do parâmetro do caminho
    /// \param flowAccLimit Valor limite referente ao acumulo de fluxo
    /// \param config Valor de configuração
    /// \param grainDispersionConfig Valor de configuração da dispersão de grão
    /// \param inputParameters Valor dos parametros de entrada
    static void writeParametersLog(QString pathParameters, int flowAccLimit, std::shared_ptr<domain::SimulationLandscapeEvolutionModelConfig> config,
                                   std::shared_ptr<domain::GrainDispersionConfig >grainDispersionConfig,
                                   std::shared_ptr<dto::LandscapeEvolutionModelInput> inputParameters);

    /// Método responsável por escrever o Log da superfície
    /// \param surfaceTitle Valor do título da superfície
    /// \param basePath Valor do caminho da base
    /// \param surface Valor da superfície
    static void writeSurfaceLog(QString surfaceTitle, QString basePath, std::shared_ptr<datamanagement::Raster<double>> surface);

    /// Método responsável por escrever o Log do fluxo acumulado
    /// \param title Valor do título
    /// \param basePath Valor do caminho base
    /// \param flowAccumulationLog Valor do Log do fluxo acumulado
    static void writeFlowAccumulationLog(QString title, QString basePath, std::shared_ptr<datamanagement::Raster<int>> flowAccumulationLog);

    /// Método responsável por escrever o Log do declive
    /// \param title Valor do título
    /// \param basePath Valor do caminho base
    /// \param slopeLog Valor do declive
    static void writeSlopeLog(QString title, QString basePath, std::shared_ptr<datamanagement::Raster<double>> slopeLog);

    /// Método responável por escrever o Log padrão d50
    /// \param title Valor do título
    /// \param basePath Valor do camiho base
    /// \param d50Log Valor do Log d50
    static void writeD50Log(QString title, QString basePath, std::shared_ptr<datamanagement::Raster<double>> d50Log);

    /// Método responsável por escrever o Log da dispersão de grãos
    /// \param title Valor do título
    /// \param basePath Valor do camiho base
    /// \param grainDispersionLog Valor do Log da dispersão de grãos
    static void writeGrainDispersionLog(QString title, QString basePath, std::shared_ptr<datamanagement::Raster<short>> grainDispersionLog);

    /// Método responsável por escrever apenas o Log da erosão e deposição
    /// \param title Valor do título
    /// \param basePath Valor do caminho base
    /// \param onlyErosionDepositionLog Valor do Log apenas da erosão e deposição
    static void writeOnlyErosionDepositionLog(QString title, QString basePath, std::shared_ptr<datamanagement::Raster<double>> onlyErosionDepositionLog);

private:
    /// Singleton.
    ProcessLandscapeEvolutionModelLogUtil();

};
} // namespace util
} // namespace landscapeevolutionmodel
} // namespace continental

#endif //CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_UTIL_ProcessLandscapeEvolutionModelLogUtil_H
