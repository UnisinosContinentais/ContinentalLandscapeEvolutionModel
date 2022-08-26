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
    /// @param simulateUntilTime Tempo de simulação em anos
    void setSimulateUntilTime(const size_t simulateUntilTime);

    /// Função de retorno da configuração dos parâmetros de dispersão do grão
    /// \return Retorna a configuração dos parâmetros de dispersão do grão
    std::shared_ptr<domain::GrainDispersionConfig> getGrainDispersionConfig() const;

    /// Função de definição da configuração dos parâmetros de dispersão do grão
    /// \param grainDispersionConfig Valor dos parâmetros de dispersão do grão
    void setGrainDispersionConfig(const std::shared_ptr<domain::GrainDispersionConfig> &grainDispersionConfig);

    /// Função de retorno da opção de habilitar/desabilitar a gravação de logs
    /// \return Retorna o valor que habilita [true] e desabilita [false]
    bool getEnableSurfaceLog() const;

    /// Função que habilita a criação de logs da superficie
    /// \param enableSurfaceLog Valor que habilita [true] e desabilita [false]
    void setEnableSurfaceLog(bool enableSurfaceLog);

    /// Função de retorno do caminho do log da superfície
    /// \return Retorna o caminho do log da superfície
    QString getPathSurfaceLog() const;

    /// Função de definição do caminho do Log da superfície
    /// \param pathSurfaceLog Valor do caminho da superficie
    void setPathSurfaceLog(const QString &pathSurfaceLog);

    /// Função de retorno com o valor da Idade
    /// \return Retorna o valor da Idade
    double getAge() const;

    /// Função de definição da Idade
    /// \param age Valor da Idade
    void setAge(double age);

    /// Função de retorno da taxa de soerguimento
    /// \return Retorna o valor da taxa de soerguimento
    std::shared_ptr<datamanagement::Raster<double> > getUpliftRate() const;

    /// Função de definição da taxa de soerguimento
    /// \param upliftRate Valor referente a taxa de soerguimento
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
