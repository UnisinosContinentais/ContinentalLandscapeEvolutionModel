/*
* Developed by UNISINOS
* author: Dante Vinicius Eloy Barbosa
* email: DANTEBARBOSA@unisinos.br
* date: March, 2020
*/
#ifndef CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_SERVICE_ERODERALGORITHMSERVICE_H
#define CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_SERVICE_ERODERALGORITHMSERVICE_H

#include "continental/landscapeevolutionmodel/export.h"
#include "continental/landscapeevolutionmodel/domain/EnumDrainageNetworkLimit.h"
#include "continental/landscapeevolutionmodel/domain/PositionMatrix.h"
#include <continental/datamanagement/Raster.h>
#include <QString>
#include <memory>
#include <cmath>
#include <vector>

namespace continental {
namespace landscapeevolutionmodel {
namespace service {

/**
 * @brief Esta classe resolve a equação da erosão transiente explícita.
 */
class CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_EXPORT_DECL EroderAlgorithmService
{
public:
    /// Construtor.
    EroderAlgorithmService();

    /// Função que retorna a proxima posição dado um ponto do grid de acordo com o grid de fluxo.
    /// @param value Valor do ponto.
    /// @param row Valor da linha.
    /// @param column Valor da coluna.
    /// @param rows Valor do número de linhas do grid.
    /// @param cols Valor do número de colunas do grid.
    bool moveToFlowDirection(short value, size_t& row, size_t& column, size_t rows, size_t cols);

    /// Função que executa o algoritimo de erodibilidade.
    void execute();

    /// Função que executa com a erosão implicita
    void executeWithImplicitErosion();

    /// Função que executa com deposição e erosão
    void executeWithErosionDeposition();

    /// Função de definição dos valores de Direção do Fluxo.
    /// @param flowDirection Objeto do tipo Raster<short>.
    void setFlowDirection(const std::shared_ptr<datamanagement::Raster<short>> flowDirection);

    /// Função de definição dos valores de Acumulação do Fluxo.
    /// @param flowAccumulation Objeto do tipo Raster<float>.
    void setFlowAccumulation(const std::shared_ptr<datamanagement::Raster<int>> flowAccumulation);

    /// Função de definição dos valores de Definição dos Rios.
    /// @param streamDefinition Objeto do tipo Raster<short>.
    void setStreamDefinition(const std::shared_ptr<datamanagement::Raster<short>> streamDefinition);

    /// Função de definição dos valores de Segmentação dos Rios.
    /// @param streamSegmentation Objeto do tipo Raster<short>.
    void setStreamSegmentation(const std::shared_ptr<datamanagement::Raster<short>> streamSegmentation);

    /// Função de definição dos valores de delimitação da bacia hidrográfica.
    /// @param catchment Objeto do tipo Raster<short>.
    void setCatchment(const std::shared_ptr<datamanagement::Raster<short>> catchment);

    /// Função de retorno do número de interação
    /// \return Retorna o valor do número de interação
    size_t getNumberOfIterations() const;

    /// Função de definição do número de interação
    /// \param numberOfIterations Valor do número de interação
    void setNumberOfIterations(const size_t &numberOfIterations);

    /// Construtor
    double getDimensionLessPrecipitationRate() const;

    /// Função de definição da taxa de precipitação adimensional
    /// \param dimensionLessPrecipitationRate Valor da taxa de precipitação adimensional
    void setDimensionLessPrecipitationRate(double dimensionLessPrecipitationRate);

    /// Função de retorno do valor Raster
    /// \return Retorna o valor do Raster
    std::shared_ptr<datamanagement::Raster<double> > getRaster() const;

    /// Função de definição do valor Rster
    /// \param raster Valor do Raster
    void setRaster(const std::shared_ptr<datamanagement::Raster<double>> &raster);

    /// Função de retorno da erodibilidade
    /// \return Retorna o valor de erodibilidade
    double getErodibility() const;

    /// Função de definição da erodibilidade
    /// \param erodibility Valor da erodibilidade
    void setErodibility(double erodibility);

    /// Função de retorno do indice de concatividade
    /// \return Retorna o valor do indice da concatividade
    double getConcavityIndex() const;

    /// Função de definição do indice de concactividade
    /// \param concavityIndex Valor do indice da concactividade
    void setConcavityIndex(double concavityIndex);

    /// Função de retorno do coeficiente de deposição adimensional
    /// \return Retorna o valor do coeficiente de deposição adimensional
    double getDimensionLessDepositionCoeficient() const;

    /// Função de definição do coeficiente de deposição adimensional
    /// \param depositionCoeficient Valor do coeficiente de deposição adimensional
    void setDimensionLessDepositionCoeficient(double depositionCoeficient);

    /// Função de retorno do delta tempo Delta_T
    /// \return Retorna o valor do delta tempo
    size_t getDeltaTime() const;

    /// Função de definição do delta tempo
    /// \param deltaTime Valor do delta tempo
    void setDeltaTime(const size_t &deltaTime);

    /// Construtor
    void useOnlyMainDrainageNetwork();

    /// Função responsável por usar a rede de drenagem limitada pela quantidade
    /// \param amountLimit Valor da rede de drenagem limitada pela quantidade
    void useDrainageNetworkAmountLimit(size_t amountLimit);

    /// Função responsável por usar a rede de drenagem limitada pela porcentagem
    /// \param percentLimit Valor da rede de drenagem limitada pela porcentagem
    void useDrainageNetworkPercentLimit(double percentLimit);

    /// Função de retorno do limite do fluxo acumulado
    /// \return Retorna o valor do limite do fluxo acumulado
    int getFlowAccumulationLimit() const;

    /// Função de definição do limite do fluxo acumulado
    /// \param flowAccumulationLimit do limite do fluxo acumulado
    void setFlowAccumulationLimit(int flowAccumulationLimit);

    /// Função de retorno do soerguimento Raster
    /// \return Retorna o valor Raster do soerguimento
    std::shared_ptr<datamanagement::Raster<double>> getUplift() const;

    /// Função de definição do valor Raster do soerguimento
    /// \param uplift Retorna o valor Raster do soerguimento
    void setUplift(const std::shared_ptr<datamanagement::Raster<double>> &uplift);

    /// Função do somatório dos doadores
    /// \param positions Valor das posições
    /// \param addUplift Valor booleano do adicional do soerguimento
    /// \return Retorna o valor do somatório dos doadores
    std::vector<std::vector<double>> donorsSummation(std::vector<domain::PositionMatrix>& positions, bool addUplift);

private:
    /// Membros.
    size_t m_numberOfCols = 0;
    size_t m_numberOfRows = 0;
    double m_cellSize = 0.0;
    size_t m_deltaTime = 0;
    double m_erodibility = 0.0;
    double m_concavityIndex = 0.0;
    /// dimensionLessPrecipitationRate representa qualquer variação na taxa de precipitação p.
    /// Pode ser descrito por p/p0, sendo p a taxa de precipitação e p0 a taxa de precipitação média.
    /// Referência: Yuan, 2019 - https://doi.org/10.1029/2018JF004867
    double m_dimensionLessPrecipitationRate = 0.2;
    double m_dimensionLessDepositionCoeficient = 1.0;
    size_t m_numberOfIterations = 20;
    domain::EnumDrainageNetworkLimit m_drainageNetworkTypeLimit = domain::EnumDrainageNetworkLimit::OnlyMain;
    size_t m_drainageNetworkAmountLimit = 0;
    double m_drainageNetworkPercentLimit = 0.0;
    int m_flowAccumulationLimit = 0;
    std::shared_ptr<datamanagement::Raster<double>> m_raster;
    std::shared_ptr<datamanagement::Raster<short>> m_flowDirection;
    std::shared_ptr<datamanagement::Raster<int>> m_flowAccumulation;
    std::shared_ptr<datamanagement::Raster<double>> m_uplift;
    std::shared_ptr<datamanagement::Raster<short>> m_streamDefinition;
    std::shared_ptr<datamanagement::Raster<short>> m_streamSegmentation;
    std::shared_ptr<datamanagement::Raster<short>> m_catchment;
};

}
}
}
#endif // CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_SERVICE_ERODERALGORITHMSERVICE_H

