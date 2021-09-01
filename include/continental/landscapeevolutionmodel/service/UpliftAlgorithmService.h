/*
* Developed by UNISINOS
* author: Dante Vinicius Eloy Barbosa
* email: DANTEBARBOSA@unisinos.br
* date: March, 2020
*/
#ifndef CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_SERVICE_UpliftAlgorithmService_H
#define CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_SERVICE_UpliftAlgorithmService_H

#include "continental/landscapeevolutionmodel/export.h"
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
class CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_EXPORT_DECL UpliftAlgorithmService
{
public:
    /// Construtor.
    UpliftAlgorithmService();

    UpliftAlgorithmService(std::shared_ptr<datamanagement::Raster<double>> initialGrid,
                           std::shared_ptr<datamanagement::Raster<double> > upliftRate,
                           double timeStep,
                           size_t numberOfTimeSteps);


    std::shared_ptr<datamanagement::Raster<double> > getInitialGrid() const;
    void setInitialGrid(const std::shared_ptr<datamanagement::Raster<double> > &initialGrid);

    double getTimeStep() const;
    void setTimeStep(double timeStep);

    std::shared_ptr<datamanagement::Raster<double> > getUpliftRate() const;
    void setUpliftRate(const std::shared_ptr<datamanagement::Raster<double> > &upliftRate);

    size_t getNumberOfTimeSteps() const;
    void setNumberOfTimeSteps(const size_t &numberOfTimeSteps);

    void executeUplift();

    std::shared_ptr<datamanagement::Raster<double>> totalUplift();

private:

    std::shared_ptr<datamanagement::Raster<double>> m_initialGrid;
    std::shared_ptr<datamanagement::Raster<double>> m_upliftRate;
    double m_timeStep = 0;
    size_t m_numberOfTimeSteps = 0;

};

}
}
}
#endif // CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_SERVICE_UpliftAlgorithmServiceq_H

