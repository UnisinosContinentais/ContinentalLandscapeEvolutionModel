#ifndef CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_SERVICE_DIRECTIONCALCULATORSERVICE_H
#define CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_SERVICE_DIRECTIONCALCULATORSERVICE_H

#include "continental/landscapeevolutionmodel/export.h"
#include "continental/landscapeevolutionmodel/domain/Direction.h"
#include "continental/landscapeevolutionmodel/domain/DrainageNetwork.h"
#include "continental/landscapeevolutionmodel/domain/EnumDrainageNetworkLimit.h"
#include "continental/landscapeevolutionmodel/domain/PositionMatrix.h"
#include <continental/datamanagement/Raster.h>
#include <memory>

namespace continental {
namespace landscapeevolutionmodel {
namespace service {

///
/// \brief The DirectionCalculatorService class Classe de Serviço responsável pelo calculo da direção
///
class CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_EXPORT_DECL DirectionCalculatorService
{
public:

    /// Método responsável pelo serviço de cálculo da direção dos rios
    /// \param flowDirection Valor do fluxo de direção
    /// \param flowAccumulation Valor do fluxo acumulado
    DirectionCalculatorService(std::shared_ptr<datamanagement::Raster<short>> flowDirection, std::shared_ptr<datamanagement::Raster<int>> flowAccumulation);

    /// Função de definição do fluxo da direção
    /// \param flowDirection Valor do fluxo da direção
    void setFlowDirection(const std::shared_ptr<datamanagement::Raster<short>> flowDirection);

    /// Função de retorno do fluxo de direção
    /// \return Retorna o fluxo de direção
    std::shared_ptr<datamanagement::Raster<short>> getFlowDirection() const;

    /// Função de retorno do fluxo acumulado
    /// \return Retorna o valor do fluxo acumulado
    std::shared_ptr<datamanagement::Raster<int>> getFlowAccumulation() const;

    /// Função de definição do fluxo acumulado
    /// \param flowAccumulation Valor do fluxo acumulado
    void setFlowAccumulation(const std::shared_ptr<datamanagement::Raster<int>> flowAccumulation);

    /// Função de retorno do processo Maps referente as posições e ramificações
    /// \return Retorna o valor do processo Maps referente as posições e ramificações
    bool getProcessMapsPositionsAndBranches() const;

    /// Função de definição do processo Maps referente as posições e ramificações
    /// \param processMapsPositionsAndBranches Valor do processo Maps referente as posições e ramificações
    void setProcessMapsPositionsAndBranches(bool processMapsPositionsAndBranches);

    /// Função de retorno do limite do fluxo acumulado
    /// \return Retorna o valor do limite do fluxo acumulado
   int getFlowAccumulationLimit() const;

    /// Função de definição do limite do fluxo do limite do fluxo acumulado
    /// \param flowAccumulationLimit Valor do limite do fluxo acumulado
    void setFlowAccumulationLimit(int flowAccumulationLimit);

    /// Função de retorno da rede de drenagem
    /// \return Retorna o valor da rede de drenagem
    std::shared_ptr<std::vector<std::shared_ptr<domain::DrainageNetwork>>> getDrainageNetworks() const;

    /// Construtor
    void execute();

    /// Construtor referente ao uso apenas da rede de drenagem principal
    void useOnlyMainDrainageNetwork();

    /// Função responsável pelo uso da rede de drenagem limitada em quantia
    /// \param amountLimit Valor limite da quantia
    void useDrainageNetworkAmountLimit(size_t amountLimit);

    /// Função responsável pelo uso da rede de drenagem limitada em porcentagem
    /// \param percentLimit Valor limite da porcentagem
    void useDrainageNetworkPercentLimit(double percentLimit);


private:
    ///Membros
    const int LimitMapPositions = std::numeric_limits<int>::max();
    std::shared_ptr<datamanagement::Raster<short>> m_flowDirection;
    std::shared_ptr<datamanagement::Raster<int>> m_flowAccumulation;
    bool m_processMapsPositionsAndBranches = false;
    int m_flowAccumulationLimit = -1;
    size_t m_rows = 0;
    size_t m_cols = 0;
	float m_noDataValue = 0.0f;
    std::shared_ptr<std::vector<std::shared_ptr<domain::DrainageNetwork>>> m_drainageNetworks;
    domain::EnumDrainageNetworkLimit m_drainageNetworkTypeLimit = domain::EnumDrainageNetworkLimit::Undefined;
    size_t m_drainageNetworkAmountLimit = 0;
    double m_drainageNetworkPercentLimit = 0.0;

    /// Função responsável por montar a árvore
    /// \param drainageNetwork Valor da rede de drenagem
    /// \param flowAccumulation Valor do fluxo acumulado
    /// \param exuterRow Valor da linha do exutório
    /// \param exuterColumn Valor da coluna do exutório
    void makeTree(domain::DrainageNetwork &drainageNetwork, datamanagement::Raster<int> &flowAccumulation, size_t exuterRow, size_t exuterColumn);

    /// Função responsável por montar a árvore
    /// \param direction Valor da direção
    /// \param flowAccumulation Valor do fluxo acumulado
    /// \param numberOfCellsOfIndentified Valor do número de células identificadas
    /// \return Retorna o valor de numberOfCellsOfIndentified
    size_t makeTree(
            domain::Direction &direction,
            datamanagement::Raster<int> &flowAccumulation,
            size_t numberOfCellsOfIndentified = 0
        );

    /// Função responsável por montar a arvore doadora
    /// \param drainageNetwork Valor da rede de drenagem
    void makeTreeDonors(domain::DrainageNetwork &drainageNetwork);

    ///
    /// \param drainageNetwork
    /// \param direction Valor da direção
    /// \param position Valor da posição
    /// \param numberOfTree Valor do número da árvore
    /// \return Retorna os valores de numberOfTree/position
    std::pair<short, size_t> makeTreeDonors(domain::DrainageNetwork &drainageNetwork, const domain::Direction &direction, size_t position, short numberOfTree);
};

} // namespace service
} // namespace landscapeevolutionmodel
} // namespace continental

#endif // CONTINENTAL_LANDSCAPEEVOLUTIONMODEL_SERVICE_DIRECTIONCALCULATORSERVICE_H
