/*
* Developed by UNISINOS - Projeto Continentais and Petrobras
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: March, 2020
*/
#ifndef CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_PROCESSLANDSCAPEEVOLUTIONMODEL_H1
#define CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_PROCESSLANDSCAPEEVOLUTIONMODEL_H1

#include <continental/datamanagement/Raster.h>
#include "continental/landscapeevolutionmodel/service/HydroToolsAlgorithmService.h"
#include "continental/landscapeevolutionmodel/service/EroderAlgorithmService.h"
#include "continental/landscapeevolutionmodel/service/DifusionAlgorithmService.h"
#include "continental/landscapeevolutionmodel/service/LithologyDefinionCalculator.h"
#include "continental/landscapeevolutionmodel/service/UpliftAlgorithmService.h"
#include "continental/landscapeevolutionmodel/dto/LandscapeEvolutionModelInput.h"
#include "continental/landscapeevolutionmodel/export.h"
#include "continental/landscapeevolutionmodel/constant/LandscapeEvolutionModelConstant.h"
#include <QString>
#include <memory>

namespace continental {
namespace landscapeevolutionmodel {

class CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_EXPORT_DECL ProcessLandscapeEvolutionModel
{
public:

    /// Função que prepara os dados de entrada para execução das interações.
    /// @param initialGrid Objeto do tipo Raster<float> com os valores de elevação topografica da simulação atual.
    /// @param inputParameters Objeto do tipo LandscapeEvolutionModelInput com os valores de parâmetros para execução do algoritimo do LEM.
    void prepare(std::shared_ptr<datamanagement::Raster<double>> initialGrid,
                  std::shared_ptr<dto::LandscapeEvolutionModelInput> inputParameters
                );

    /// Função que executa o proximo passo de tempo.
    /// @return Valor boleano com [true] para continuar para proximo passo de tempo [false] não existem mais passo de tempo a ser processado.
    bool iterate();

    /// Função de retorno do resultado da Simulação.
    /// @return Objeto do tipo Raster<float> com resultado final da simulação.
    std::shared_ptr<datamanagement::Raster<double>> getResultSimulation();

    std::shared_ptr<datamanagement::Raster<short>> getResultDispersionSimulation();

    /// Função de retorno do sub-passo de tempo da Simulação atual.
    /// @return Valor do tipo size_t com o valor do sub-passo que esta sendo processado.
    size_t getSimulationTimeStep();

    void calculateOnlyErosionDepositionGrid();

private :
    /// Membros.
    size_t m_simulateUntilTime = 0;
    size_t m_difusionDeltaT = constant::LandscapeEvolutionModelConstant::DifusionDeltaT;
    double m_erosionDeltaT = constant::LandscapeEvolutionModelConstant::ErosionDeltaT;
    size_t m_timeStepCount = 0;
    int m_flowAccumulationLimit = 0;

    bool m_enableSurfaceLog = false;
    QString m_logSurfacePath;
    double m_logAge = 0.0;
    int m_logNode = 0;

    std::shared_ptr<datamanagement::Raster<double>> m_surface;
    std::shared_ptr<datamanagement::Raster<short>> m_grainDispersion;
    std::shared_ptr<dto::LandscapeEvolutionModelInput> m_inputParameters;
    service::HydroToolsAlgorithmService m_hydroToolsAlgorithm;
    service::DifusionAlgorithmService m_difusionAlgorithm;
    service::EroderAlgorithmService m_eroderAlgorithm;
    service::LithologyDefinionCalculator m_grainDispersionService;
    service::UpliftAlgorithmService m_upliftAlgorithm;

    std::shared_ptr<datamanagement::Raster<double>> m_totalUplift;

    //nomear para ser o grid que possui as erosões e deposições
    std::shared_ptr<datamanagement::Raster<double>> m_onlyErosionDepositionGrid;

    /// Função que valida se a interação é valida.
    void validateInterate();
    void prepareFlowAccumulationLimit();
};

} // landscapeevolutionmodel
} // continental
#endif // CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_PROCESSLANDSCAPEEVOLUTIONMODEL_H
