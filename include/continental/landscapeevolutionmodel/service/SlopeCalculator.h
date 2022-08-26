/*
* Developed by UNISINOS - Projeto Continentais and Petrobras
* author: Rudi C. C. Modena
* email: rmodena@unisinos.br
* date: October, 2020
*/
#ifndef CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_SERVICE_SLOPECALCULATOR_H
#define CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_SERVICE_SLOPECALCULATOR_H

#define _USE_MATH_DEFINES
#include <cmath>
#include "continental/landscapeevolutionmodel/export.h"
#include "continental/landscapeevolutionmodel/domain/definition/SlopeUnits.h"
#include "continental/landscapeevolutionmodel/domain/definition/SlopeTypes.h"
#include <continental/datamanagement/Raster.h>
#include <memory>
#include <vector>

namespace continental {
namespace landscapeevolutionmodel {
namespace service {

///
/// \brief The SlopeCalculator class Classe responsável pelo cálculo do declive
///
class CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_EXPORT_DECL SlopeCalculator
{

public:

    /// Função de retorno da entrada do Raster
    /// \return Retorna o valor de entrada do Raster
    std::shared_ptr<continental::datamanagement::Raster<double> > getInputRaster() const;

    /// Função de definição da entrada do Raster
    /// \param inputRaster Valor da entrada do Raster
    void setInputRaster(const std::shared_ptr<continental::datamanagement::Raster<double> > inputRaster);

    /// Função de retorno da saída do Raster
    /// \return Retorna o valor de saída do Raster
    std::shared_ptr<continental::datamanagement::Raster<double> > getOutputRaster() const;

    /// Função de definição da saída do Raster
    /// \param outputRaster Valor da saída do Raster
    void setOutputRaster(const std::shared_ptr<continental::datamanagement::Raster<double> > outputRaster);

    /// Função de retorno da unidade do declive
    /// \return Retorna o valor da unidade
    continental::landscapeevolutionmodel::domain::definition::SlopeUnits getSlopeUnit() const;

    /// Função de definição da unidade do declive
    /// \param unit Valor da unidade
    void setSlopeUnit(const continental::landscapeevolutionmodel::domain::definition::SlopeUnits unit);

    /// Função de retorno do tipo de declividade
    /// \return Retorna o valor do tipo de declividade
    continental::landscapeevolutionmodel::domain::definition::SlopeType getSlopeType() const;

    /// Função de definição do tipo de declividade
    /// \param slopeType Valor do tipo de declive
    void setSlopeType(const continental::landscapeevolutionmodel::domain::definition::SlopeType slopeType);

    /// Construtor
    void calculateSlopeRaster();

private:
    /// Membros.
    std::shared_ptr<continental::datamanagement::Raster<double>> m_inputRaster;
    std::shared_ptr<continental::datamanagement::Raster<double>> m_outputRaster;
    continental::landscapeevolutionmodel::domain::definition::SlopeUnits m_slopeUnit = continental::landscapeevolutionmodel::domain::definition::SlopeUnits::tangent;
    continental::landscapeevolutionmodel::domain::definition::SlopeType m_slopeType = continental::landscapeevolutionmodel::domain::definition::SlopeType::slopeHorn1981;

    /// Função responsável pela janela móvel
    /// \param iPosition Valor da posição I
    /// \param jPosition Valor da posição J
    /// \return Retorna as posições da janela móvel
    std::shared_ptr<std::vector<std::vector<float> >> generateMovingWindow(const size_t iPosition, const size_t jPosition) const;

    /// Função responsável pelo cálculo da declividade de acordo com o valor de Horn1981
    /// \param movingWindow Valor da janela móvel
    /// \return Retorno do valor calculado para Horn1981
    float calculateSlopeHorn1981Value(const std::shared_ptr<std::vector<std::vector<float> >> movingWindow) const;

    /// Função responsável pelo calculo da máxima declividade
    /// \param movingWindow Valor da janela móvel
    /// \return Retorna o Valor calculado da máxima declividade
    float calculateMaxSlope(const std::shared_ptr<std::vector<std::vector<float> >> movingWindow) const;

    /// Função responsável por converter a unidade em tangente, porcentagem, grau ou radiano
    /// \param slopeTangent Valor da tangente do declive
    /// \return Retorna o valor convertido desejado
    float convertUnit(const float slopeTangent) const;
};

}
}
}

#endif // CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_SERVICE_SLOPECALCULATOR_H
