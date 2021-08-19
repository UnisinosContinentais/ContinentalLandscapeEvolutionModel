#include "continental/landscapeevolutionmodel/util/ProcessLandscapeEvolutionModelLogUtil.h"
#include "continental/landscapeevolutionmodel/domain/SimulationLandscapeEvolutionModelConfig.h"
#include "continental/landscapeevolutionmodel/domain/GrainDispersionConfig.h"
#include "continental/landscapeevolutionmodel/dto/LandscapeEvolutionModelInput.h"
#include <continental/datamanagement/RasterFile.h>

#include <QString>
#include <QFile>
#include <QTextStream>

namespace continental {
namespace landscapeevolutionmodel {
namespace util {

ProcessLandscapeEvolutionModelLogUtil::ProcessLandscapeEvolutionModelLogUtil() = default;


void ProcessLandscapeEvolutionModelLogUtil::writeParametersLog(QString pathParameters,
                                                               int flowAccLimit, std::shared_ptr<domain::SimulationLandscapeEvolutionModelConfig> config,
                                                               std::shared_ptr<domain::GrainDispersionConfig >grainDispersionConfig,
                                                               std::shared_ptr<dto::LandscapeEvolutionModelInput> inputParameters)
{
    QFile file(pathParameters);
    if (file.open(QFile::WriteOnly | QFile::Truncate))
    {
        QTextStream out(&file);

        out << "diffusivity: " << QString::number(config->getDiffusivity(), 'f', 10) << "\n";
        out << "erodibility: " << QString::number(config->getErodibility(), 'f', 10) << "\n";
        out << "concavityIndex: " << QString::number(config->getConcavityIndex(), 'f', 10) << "\n";
        out << "precipitationRate: " << QString::number(config->getDimensionLessPrecipitationRate(), 'f', 10) << "\n";
        out << "depositionCoeficient: " << QString::number(config->getDimensionLessDepositionCoeficient(), 'f', 10) << "\n";
        out << "drainageNetworkTypeLimit: ";
        switch (config->getDrainageNetworkTypeLimit())
        {
            case domain::OnlyMain:
                out << "OnlyMain";
                break;
            case domain::Percent:
                out << "Percent";
                break;
            case domain::Amount:
                out << "Amount";
                break;
            default:
                out << "Undefined";
                break;
        } //pq aqui pega de m_grainDispersionService e não de grainDispersionConfig ?
        out << "drainageNetworkAmountLimit: " << QString::number(config->getDrainageNetworkAmountLimit()) << "\n";
        out << "drainageNetworkPercentLimit: " << QString::number(config->getDrainageNetworkPercentLimit()) << "\n";
        out << "drainageNetworkAmountLimit: " << QString::number(config->getDrainageNetworkAmountLimit()) << "\n";
        out << "drainageNetworkPercentLimit: " << QString::number(config->getDrainageNetworkPercentLimit()) << "\n";
        out << "simulateUntilTime: " << QString::number(inputParameters->getSimulateUntilTime()) << "\n";
        out << "facLimit: " << QString::number(flowAccLimit) << "\n";
        out << "channelDepthCParameter: " << grainDispersionConfig->getChannelDepthCParameter() << "\n";
        out << "channelDepthFParameter: " << grainDispersionConfig->getChannelDepthFParameter() << "\n";
        out << "dischargeEParameter: " << grainDispersionConfig->getDischargeEParameter() << "\n";
        out << "dischargeKParameter: " << grainDispersionConfig->getDischargeKParameter() << "\n";
        out << "grainSizeWaterDensity: " << grainDispersionConfig->getGrainSizeWaterDensity() << "\n";
        out << "grainSizeShieldsNumber: " << grainDispersionConfig->getGrainSizeShieldsNumber() << "\n";
        out << "grainSizeSedimentDensity: " << grainDispersionConfig->getGrainSizeSedimentDensity() << "\n";
    }

}

void ProcessLandscapeEvolutionModelLogUtil::writeSurfaceLog(QString title, QString basePath, std::shared_ptr<datamanagement::Raster<double>> surface)
{
    QString path = basePath + title + ".asc";
    datamanagement::RasterFile<double>::writeData(*surface, path);
}

void ProcessLandscapeEvolutionModelLogUtil::writeFlowAccumulationLog(QString title, QString basePath, std::shared_ptr<datamanagement::Raster<int>>flowAccumulationLog)
{
    QString path = basePath + title + ".asc";
    datamanagement::RasterFile<int>::writeData(*flowAccumulationLog, path);
}

void ProcessLandscapeEvolutionModelLogUtil::writeSlopeLog(QString title, QString basePath, std::shared_ptr<datamanagement::Raster<double>> slopeLog)
{
    QString path = basePath + title + ".asc";
    datamanagement::RasterFile<double>::writeData(*slopeLog, path);
}

void ProcessLandscapeEvolutionModelLogUtil::writeD50Log(QString title, QString basePath, std::shared_ptr<datamanagement::Raster<double>> d50Log)
{
    QString path = basePath + title + ".asc";
    datamanagement::RasterFile<double>::writeData(*d50Log, path);
}

void ProcessLandscapeEvolutionModelLogUtil::writeGrainDispersionLog(QString title, QString basePath, std::shared_ptr<datamanagement::Raster<short>> grainDispersionLog)
{
    QString path = basePath + title + ".asc";
    datamanagement::RasterFile<short>::writeData(*grainDispersionLog, path);
}

}
}
}

