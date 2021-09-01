/*
* Developed by UNISINOS - Projeto Continentais and Petrobras
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: March, 2020
*/
#ifndef CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_DTO_LANDSCAPEEVOLUTIONMODELINPUT_H
#define CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_DTO_LANDSCAPEEVOLUTIONMODELINPUT_H

#include <continental/datamanagement/Raster.h>
#include <memory>
#include <QString>
#include "../export.h"

namespace continental
{
namespace landscapeevolutionmodel
{
    namespace domain
    {
        class SinkDestroyConfig;
        class StreamDefinitionConfig;
        class SimulationLandscapeEvolutionModelConfig;
        class GrainDispersionConfig;
    }
namespace dto {

/**
 * @brief Classe de definição dos parâmetros de entrada do processo do Modelo de Evolução Topográfica(LEM).
 * Esta classe contém funções para configurar os parâmetros de entrada do processo Landscape Evolution Model.
 */
class CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_EXPORT_DECL LandscapeEvolutionModelInput
{
public:
    /// Construtor.
    LandscapeEvolutionModelInput();

    /// Função de retorno da configuração utilizados pelo processo de Sink Destroy.
    /// @return Objeto do tipo SinkDestroyConfig com os dados de configuração.
    std::shared_ptr<domain::SinkDestroyConfig> getSinkDestroyConfig() const;

    /// Função de definição da configuração utilizados pelo processo de Sink Destroy.
    /// @param sinkDestroyConfig Objeto do tipo SinkDestroyConfig com dados de configuração.
    void setSinkDestroyConfig(const std::shared_ptr<domain::SinkDestroyConfig> &sinkDestroyConfig);

    /// Função de retorno  da configuração utilizados pelo processo de Definição e segmentação dos Rios.
    /// @return Objeto do tipo StreamDefinitionConfig com os dados de configuração.
    std::shared_ptr<domain::StreamDefinitionConfig> getStreamDefinitionConfig() const;

    /// Função de definição da configuração utilizados pelo processo de Definição e segmentação dos Rios.
    /// @param streamDefinitionConfig Objeto do tipo StreamDefinitionConfig com dados de configuração.
    void setStreamDefinitionConfig(const std::shared_ptr<domain::StreamDefinitionConfig> &streamDefinitionConfig);

    /// Função de retorno da configuração utilizados pelo processo LEM.
    /// @return Objeto do tipo SimulationLandscapeEvolutionModelConfig com os dados de configuração.
    std::shared_ptr<domain::SimulationLandscapeEvolutionModelConfig> getSimulationLandscapeEvolutionModelConfig() const;

    /// Função de definição da configuração utilizados pelo processo LEM.
    /// @param simulationLandscapeEvolutionModelConfig Objeto do tipo SimulationLandscapeEvolutionModelConfig com dados de configuração.
    void setSimulationLandscapeEvolutionModelConfig(const std::shared_ptr<domain::SimulationLandscapeEvolutionModelConfig> &simulationLandscapeEvolutionModelConfig);

    /// Função de retorno do tempo de simulação em anos
    /// @return Tempo de simulação em anos
    size_t getSimulateUntilTime() const;

    /// Função de definição do tempo de simulação em anos
    /// @param Tempo de simulação em anos
    void setSimulateUntilTime(const size_t simulateUntilTime);

    std::shared_ptr<domain::GrainDispersionConfig> getGrainDispersionConfig() const;
    void setGrainDispersionConfig(const std::shared_ptr<domain::GrainDispersionConfig> &grainDispersionConfig);

    bool getEnableSurfaceLog() const;
    void setEnableSurfaceLog(bool enableSurfaceLog);

    QString getPathSurfaceLog() const;
    void setPathSurfaceLog(const QString &pathSurfaceLog);

    double getAge() const;
    void setAge(double age);

    std::shared_ptr<datamanagement::Raster<double> > getUpliftRate() const;
    void setUpliftRate(const std::shared_ptr<datamanagement::Raster<double> > &upliftRate);

private:
    /// Membros.
    std::shared_ptr<domain::SinkDestroyConfig> m_sinkDestroyConfig;
    std::shared_ptr<domain::StreamDefinitionConfig> m_streamDefinitionConfig;
    std::shared_ptr<domain::SimulationLandscapeEvolutionModelConfig> m_simulationLandscapeEvolutionModelConfig;
    std::shared_ptr<domain::GrainDispersionConfig> m_grainDispersionConfig;
    std::shared_ptr<datamanagement::Raster<double>> m_upliftRate;
    size_t m_simulateUntilTime = 0;
    double m_age = 0.0;
    bool m_enableSurfaceLog = false;
    QString m_pathSurfaceLog;
};

} //dto
} //landscapeevolutionmodel
} //continental


#endif // CONTINENTALPLUGIN_DTO_FLUVIAL_SYSTEM_TRANSIENTSURFACEBEDROCKINPUT_H
