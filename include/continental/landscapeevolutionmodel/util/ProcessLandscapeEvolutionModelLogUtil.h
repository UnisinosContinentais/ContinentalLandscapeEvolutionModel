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


    static void writeParametersLog(QString pathParameters, int flowAccLimit, std::shared_ptr<domain::SimulationLandscapeEvolutionModelConfig> config,
                                   std::shared_ptr<domain::GrainDispersionConfig >grainDispersionConfig,
                                   std::shared_ptr<dto::LandscapeEvolutionModelInput> inputParameters);



    static void writeSurfaceLog(QString surfaceTitle, QString basePath, std::shared_ptr<datamanagement::Raster<double>> surface);


    static void writeFlowAccumulationLog(QString title, QString basePath, std::shared_ptr<datamanagement::Raster<int>> flowAccumulationLog);

    static void writeSlopeLog(QString title, QString basePath, std::shared_ptr<datamanagement::Raster<double>> slopeLog);

    static void writeD50Log(QString title, QString basePath, std::shared_ptr<datamanagement::Raster<double>> d50Log);

    static void writeGrainDispersionLog(QString title, QString basePath, std::shared_ptr<datamanagement::Raster<short>> grainDispersionLog);

private:
    /// Singleton.
    ProcessLandscapeEvolutionModelLogUtil();

};
} // namespace util
} // namespace landscapeevolutionmodel
} // namespace continental

#endif //CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_UTIL_ProcessLandscapeEvolutionModelLogUtil_H
