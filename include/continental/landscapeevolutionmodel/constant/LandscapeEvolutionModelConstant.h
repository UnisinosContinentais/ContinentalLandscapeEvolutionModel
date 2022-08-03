/*
* Developed by UNISINOS - Projeto Continentais and Petrobras
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: March, 2020
*/
#ifndef CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_CONSTANT_LANDSCAPEVOLUTIONMODELCONSTANT_H
#define CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_CONSTANT_LANDSCAPEVOLUTIONMODELCONSTANT_H

#include <QString>
#include "continental/landscapeevolutionmodel/export.h"

namespace continental {
namespace landscapeevolutionmodel {
namespace constant {

/** @brief Classe de constantes.
 * Esta classe contém variáveis constantes usadas pelo projeto.
 */
class CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_EXPORT_DECL LandscapeEvolutionModelConstant
{
public:
    constexpr static const short NoDataValue = -9999;

    //Parametros bibliograficos
    constexpr static const double ParametersSimulationErodibility = 0.00002;
    constexpr static const double ParametersSimulationDiffusivity = 0.01;
    constexpr static const double ParametersSimulationValueConcavityIndex = 0.4; // concavity index
    constexpr static const double ParametersSimulationValueN = 1.0; // // slop index
    constexpr static const double ParametersSimulationDimensionLessPrecipitationRate = 0.2;
    constexpr static const double ParametersSimulationDimensionLessDepositionCoeficient = 1.0;

    constexpr static const double BoundaryConditionNorth = 0;
    constexpr static const double BoundaryConditionWest = 0;
    constexpr static const double BoundaryConditionSouth = 0;
    constexpr static const double BoundaryConditionEast = 0;

    constexpr static const size_t ParametersSinkDestroyConfigMaxOpenList = 1000000;
    constexpr static const size_t ParametersSinkDestroyConfigMaxClosedList = 500000;
    constexpr static const size_t ParametersSinkDestroyConfigWeightFunctionCost = 2.00;

    constexpr static const double ParametersStreamDefinitionConfigAreaThreshold = 50.0f;
    static const bool ParametersStreamDefinitionConfigApproximateArea = false;
    static const bool ParametersStreamDefinitionConfigPercentualOfNumberOfCells = true;

    //Constantes dos Processos
    constexpr static const size_t DifusionDeltaT = 1000; // ano
    constexpr static const double ErosionDeltaT = 1000; // ano
    constexpr static const size_t SaveEachDeltaT = 1000; // ano
    constexpr static const size_t SimulateUntilTime = 10000;

    //Constantes para o cálculo da dispersão
    constexpr static const double DischargeKParameter = 0.08;
    constexpr static const double DischargeEParameter = 0.95;
    constexpr static const double ChannelDepthCParameter = 0.27;
    constexpr static const double ChannelDepthFParameter = 0.3;

    //Constantes para o cálculo do tamanho do grão
    constexpr static const double GrainSizeWaterDensity = 1000; // kg/m3
    constexpr static const double GrainSizeSedimentDensity = 2650; // kg/m3
    constexpr static const double GrainSizeShieldsNumber = 0.03; // adimensional

    //Constantes para os tamanhos de grãos em milímetros
    // constexpr static const double MaxGranuleSize = 4.0; Acima de Areia Grossa todos os tamanhos foram carimbados como Gravel
    constexpr static const double MaxCoarseSandSize = 1.0;
    constexpr static const double MaxMediumSandSize = 0.5;
    constexpr static const double MaxFineSandSize = 0.25;
    constexpr static const double MaxSiltSize = 0.062;
    constexpr static const double MaxClaySize = 0.004;

    //Parâmetro para salvar os logs
     constexpr static const bool EnableLogs = true;

    //Parâmetro para transformação de M^3/passo de tempo em Anos por anos


    //ACHO QUE PRECISA LIGAR UMA VARIÁVEL COM O PATH ONDE QUE QUE SALVE
    //constexpr static const QString LogsBasePath = "C:/Users/ADM_LOCAL/Desktop/teste_logs"; //ainda não está sendo usado

};

}
}
}

#endif // CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_CONSTANT_LANDSCAPEVOLUTIONMODELCONSTANT_H
