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
 * Lembrete: parâmetro para transformação de M³/passo de tempo em Anos por anos
 */
class CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_EXPORT_DECL LandscapeEvolutionModelConstant
{
public:

    /// Membros
    /// Abaixo segue algumas orientações:
    /// 1. NoDataValue valor definido sem valor para operações com as matrizes
    /// 2. As constantes abaixo são referente aos parametros bibliograficos
    /// 3. Os valores DifusionDeltaT, ErosionDeltaT e SaveEachDeltaT são referenciado em ano
    /// 4. Os valores de GrainSizeWaterDensity e GrainSizeSedimentDensity possue a unidade em kg/m³
    /// 5. GrainSizeShieldsNumber tem a unidade adimensional
    /// 6. Constantes para os tamanhos de grãos em milímetro
    ///
    constexpr static const short NoDataValue = -9999;
    constexpr static const double ParametersSimulationErodibility = 0.00002;
    constexpr static const double ParametersSimulationDiffusivity = 0.01;
    constexpr static const double ParametersSimulationValueConcavityIndex = 0.4;
    constexpr static const double ParametersSimulationValueN = 1.0;
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
    constexpr static const size_t DifusionDeltaT = 1000;
    constexpr static const double ErosionDeltaT = 1000;
    constexpr static const size_t SaveEachDeltaT = 1000;
    constexpr static const size_t SimulateUntilTime = 10000;
    constexpr static const double DischargeKParameter = 0.08;
    constexpr static const double DischargeEParameter = 0.95;
    constexpr static const double ChannelDepthCParameter = 0.27;
    constexpr static const double ChannelDepthFParameter = 0.3;
    constexpr static const double GrainSizeWaterDensity = 1000;
    constexpr static const double GrainSizeSedimentDensity = 2650;
    constexpr static const double GrainSizeShieldsNumber = 0.03;
    constexpr static const double MaxCoarseSandSize = 1.0;
    constexpr static const double MaxMediumSandSize = 0.5;
    constexpr static const double MaxFineSandSize = 0.25;
    constexpr static const double MaxSiltSize = 0.062;
    constexpr static const double MaxClaySize = 0.004;
    // constexpr static const double MaxGranuleSize = 4.0; Acima de Areia Grossa todos os tamanhos foram carimbados como Gravel
    constexpr static const bool EnableLogs = true;

    //ACHO QUE PRECISA LIGAR UMA VARIÁVEL COM O PATH ONDE QUE QUE SALVE
    //constexpr static const QString LogsBasePath = "C:/Users/ADM_LOCAL/Desktop/teste_logs"; //ainda não está sendo usado

};

}
}
}

#endif // CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_CONSTANT_LANDSCAPEVOLUTIONMODELCONSTANT_H
