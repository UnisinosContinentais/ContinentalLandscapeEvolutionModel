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
#include <iostream>

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

    /// Construtor.
    /// @param initialGrid Objeto do tipo Raster<float> com a Superfície Inicial.
    /// @param simulateUtilTime Valor tipo size_t com o tempo total de simulação em anos .
    /// @param erodibility Valor do tipo double com o parametro de erodibilidade.
    /// @param deltaT Valor tipo size_t com a o passo de tempo em anos.
    /// @param concavityIndex Valor do tipo double com o valor indice de concavidade da bacia.
    EroderAlgorithmService(std::shared_ptr<datamanagement::Raster<float>> raster, double erodibility, size_t deltaT, double concavityIndex);

    /// Função que retorna a proxima posição dado um ponto do grid de acordo com o grid de fluxo.
    /// @param value Valor do ponto.
    /// @param row Valor da linha.
    /// @param column Valor da coluna.
    /// @param rows Valor do número de linhas do grid.
    /// @param cols Valor do número de colunas do grid.
    bool moveToFlowDirection(short value, size_t& row, size_t& column, size_t rows, size_t cols);

    /// Função que executa o algoritimo de erodibilidade.
    void execute();

    void executeWithImplicitErosion();

    void executeWithErosionDeposition();

    /// Função de definição dos valores de Direção do Fluxo.
    /// @param flowDirection Objeto do tipo Raster<short>.
    void setFlowDirection(const std::shared_ptr<datamanagement::Raster<short>> flowDirection);

    /// Função de definição dos valores de Acumulação do Fluxo.
    /// @param flowAccumulation Objeto do tipo Raster<float>.
    void setFlowAccumulation(const std::shared_ptr<datamanagement::Raster<float>> flowAccumulation);

    /// Função de definição dos valores de Definição dos Rios.
    /// @param streamDefinition Objeto do tipo Raster<short>.
    void setStreamDefinition(const std::shared_ptr<datamanagement::Raster<short>> streamDefinition);

    /// Função de definição dos valores de Segmentação dos Rios.
    /// @param streamSegmentation Objeto do tipo Raster<short>.
    void setStreamSegmentation(const std::shared_ptr<datamanagement::Raster<short>> streamSegmentation);

    /// Função de definição dos valores de delimitação da bacia hidrográfica.
    /// @param catchment Objeto do tipo Raster<short>.
    void setCatchment(const std::shared_ptr<datamanagement::Raster<short>> catchment);

    size_t getNumberOfIterations() const;

    void setNumberOfIterations(const size_t &numberOfIterations);

    double getPrecipitationRate() const;

    void setPrecipitationRate(double precipitationRate);

    std::shared_ptr<datamanagement::Raster<float> > getRaster() const;

    void setRaster(const std::shared_ptr<datamanagement::Raster<float>> &raster);

    double getErodibility() const;

    void setErodibility(double erodibility);

    double getConcavityIndex() const;

    void setConcavityIndex(double concavityIndex);

    double getDepositionCoeficient() const;

    void setDepositionCoeficient(double depositionCoeficient);

    size_t getDeltaTime() const;

    void setDeltaTime(const size_t &deltaTime);

    void useOnlyMainDrainageNetwork();

    void useDrainageNetworkAmountLimit(size_t amountLimit);

    void useDrainageNetworkPercentLimit(double percentLimit);

    short getFlowAccumulationLimit() const;

    void setFlowAccumulationLimit(short flowAccumulationLimit);

    std::shared_ptr<datamanagement::Raster<double>> getUplift() const;

    void setUplift(const std::shared_ptr<datamanagement::Raster<double>> &uplift);

    std::vector<std::vector<double>> donorsSummation(std::vector<domain::PositionMatrix>& positions, bool addUplift);

private:
    /// Membros.
    size_t m_numberOfCols = 0;
    size_t m_numberOfRows = 0;
    double m_cellSize = 0.0;
    size_t m_deltaTime = 0;
    double m_erodibility = 0.0;
    double m_concavityIndex = 0.0;
    double m_precipitationRate = 0.2;
    double m_depositionCoeficient = 1.0;
    size_t m_numberOfIterations = 20;
    domain::EnumDrainageNetworkLimit m_drainageNetworkTypeLimit = domain::EnumDrainageNetworkLimit::OnlyMain;
    size_t m_drainageNetworkAmountLimit = 0;
    double m_drainageNetworkPercentLimit = 0.0;
    short m_flowAccumulationLimit = 0;
    std::shared_ptr<datamanagement::Raster<float>> m_raster;
    std::shared_ptr<datamanagement::Raster<short>> m_flowDirection;
    std::shared_ptr<datamanagement::Raster<float>> m_flowAccumulation;
    std::shared_ptr<datamanagement::Raster<double>> m_uplift;
    std::shared_ptr<datamanagement::Raster<short>> m_streamDefinition;
    std::shared_ptr<datamanagement::Raster<short>> m_streamSegmentation;
    std::shared_ptr<datamanagement::Raster<short>> m_catchment;
};

}
}
}
#endif // CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_SERVICE_ERODERALGORITHMSERVICE_H

