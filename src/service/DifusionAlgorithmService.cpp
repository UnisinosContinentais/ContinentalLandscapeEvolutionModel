#include "continental/landscapeevolutionmodel/service/DifusionAlgorithmService.h"
#include <continental/hydrotools/service/HeuristicSinkRemoval.h>
#include <continental/hydrotools/service/HeuristicSinkRemovalUtil.h>
#include <continental/hydrotools/service/FlowAccumulation.h>
#include <continental/hydrotools/service/StreamDefinition.h>
#include <continental/hydrotools/service/StreamSegmentation.h>
#include <continental/hydrotools/service/Catchment.h>
#include <QString>
#include <memory>

using namespace std;
using namespace continental::hydrotools;
using namespace continental::datamanagement;

namespace continental {
namespace landscapeevolutionmodel {
namespace service {

DifusionAlgorithmService::DifusionAlgorithmService()
{

}

DifusionAlgorithmService::DifusionAlgorithmService(std::shared_ptr<Raster<double>> initialGrid, double diffusivity, size_t deltaT) :
    m_initialGrid(initialGrid),
    m_deltaT(deltaT),
    m_diffusivity(diffusivity)
{
    m_numberOfCols = m_initialGrid->getCols();
    m_numberOfRows = m_initialGrid->getRows();
    m_deltaX = m_initialGrid->getCellSize();
    m_deltaY = m_initialGrid->getCellSize();
    m_numberOfIterations = 10;
}

void DifusionAlgorithmService::allocateTopography()
{
    m_T.resize(m_numberOfRows);
#pragma omp parallel for
    for (int iAux = 0; iAux < static_cast<int>(m_numberOfRows); ++iAux)
    {
        size_t i = static_cast<size_t>(iAux);
        m_T[i].resize(m_numberOfCols);
        for (size_t j = 0; j < m_numberOfCols; ++j)
        {
            m_T[i][j].resize(m_numberOfIterations);
            for (size_t it = 0; it < m_numberOfIterations; ++it)
            {
                m_T[i][j][it].resize(2);
            }
        }
    }
}


void DifusionAlgorithmService::execute()
{

    // condições de contornos

    // condição de isolamento
    double fluxAtLimit = 0.0;

    // condição de variável prescrita
    double valueAtBorder = 50;

    // t = 1 para indicar que o dado é do tempo futuro.
    size_t t = 1;

    // contador do numero de passo de tempo para os processos iterativos
    size_t timeStepCount = 0; // Acho que não está sendo usado

    // Equação Geral: ap * T = aW * TW + aE* TE + aN * TN + aS * TS

    // calculo dos coeficientes da matriz
    // cada nó do domínio possui um quatro vizinhos (East, West, North e Sauth)
    double aE = m_diffusivity * m_deltaY / m_deltaX;
    double aW = m_diffusivity * m_deltaY / m_deltaX;
    double aS = m_diffusivity * m_deltaY / m_deltaX;
    double aN = m_diffusivity * m_deltaY / m_deltaX;

    // Quando o vizinho do nó em estudo é uma fronteira
    // aplica-se 0 para fronteira isolada
    // quando o vizinho de fronteira possui variavel prescrita, reliza-se o balanço
    double aEB = 0.0;
    double aWB = 0.0;
    double aSB = 2 * m_diffusivity * m_deltaX / m_deltaY;
    double aNB = 0;

    // O grid de quadrilateros estruturados possuem 9 tipos de volmes
    // 4 tipos de volumes de cantos, 4 tipos de volumes de bordas
    // 1 tipo de volume rodeado de volumes
    double ap0 = m_deltaY * m_deltaX / m_deltaT;
    double ap1 = aE + aWB + aN + aSB + ap0;
    double ap2 = aE + aW + aN + aSB + ap0;
    double ap3 = aEB + aW + aN + aSB + ap0;
    double ap4 = aE + aWB + aN + aS + ap0;
    double ap5 = aE + aW + aN + aS + ap0;
    double ap6 = aEB + aW + aN + aS + ap0;
    double ap7 = aE + aWB + aNB + aS + ap0;
    double ap8 = aE + aW + aNB + aS + ap0;
    double ap9 = aEB + aW + aNB + aS + ap0;

    // determina condição inicial como sendo a topografia analisada
    for (size_t i = 0, iAux = m_numberOfRows - 1; i < m_numberOfRows; ++i, --iAux)
    {
        for (size_t j = 0; j < m_numberOfCols; ++j)
        {
            m_T[i][j][0][0] = m_initialGrid->getData(iAux, j);
        }
    }

    // arbitra o resultado da topografia no proximo passo de tempo
    for (size_t i = 0; i < m_numberOfRows; ++i)
    {
        for (size_t j = 0; j < m_numberOfCols; ++j)
        {
            m_T[i][j][0][t] = m_T[i][j][0][0];
        }
    }

    // Partindo do valor arbitrado começa o método de solução iterativo do sistema de equações
    for (size_t it = 1; it < m_numberOfIterations; ++it)
    {
        for (size_t i = 0; i < m_numberOfRows; ++i)
        {
            // Volumes de canto South-West, borda Sul e canto South-East
            if (i == 0)
            {
                for (size_t j = 0; j < m_numberOfCols; ++j)
                {
                    // corner South-West
                    if (j == 0)
                    {
                        m_T[i][j][it][t] = (aE * m_T[i][j + 1][it - 1][t] + aN * m_T[i + 1][j][it - 1][t] + aSB * valueAtBorder + ap0 * m_T[i][j][0][t - 1]) / ap1;
                    }
                    // borda South
                    if (j > 0 && j < m_numberOfCols - 1)
                    {
                        m_T[i][j][it][t] = (aW * m_T[i][j - 1][it - 1][t] + aE * m_T[i][j + 1][it - 1][t] + aN * m_T[i + 1][j][it - 1][t] + aSB * valueAtBorder + ap0 * m_T[i][j][0][t - 1]) / ap2;
                    }
                    // corner South-East
                    if (j == m_numberOfCols - 1)
                    {
                        m_T[i][j][it][t] = (aW * m_T[i][j - 1][it - 1][t] + aN * m_T[i + 1][j][it - 1][t] + aSB * valueAtBorder + ap0 * m_T[i][j][0][t - 1]) / ap3;
                    }
                }
            }
            // volumes de borda West, volume genérico, volume de borda East
            if (i > 0 && i < m_numberOfRows - 1)
            {
                for (size_t j = 0; j < m_numberOfCols; ++j)
                {
                    // borda West
                    if (j == 0)
                    {
                        m_T[i][j][it][t] = (aE * m_T[i][j + 1][it - 1][t] + aN * m_T[i + 1][j][it - 1][t] + aS * m_T[i - 1][j][it - 1][t] + ap0 * m_T[i][j][0][t - 1]) / ap4;
                    }
                    // volume generico
                    if (j > 0 && j < m_numberOfCols - 1)
                    {
                        m_T[i][j][it][t] = (aE * m_T[i][j + 1][it - 1][t] + aW * m_T[i][j - 1][it - 1][t] + aN * m_T[i + 1][j][it - 1][t] + aS * m_T[i - 1][j][it - 1][t] + ap0 * m_T[i][j][0][t - 1]) / ap5;
                    }
                    // borda East
                    if (j == m_numberOfCols - 1)
                    {
                        m_T[i][j][it][t] = (aW * m_T[i][j - 1][it - 1][t] + aN * m_T[i + 1][j][it - 1][t] + aS * m_T[i - 1][j][it - 1][t] + ap0 * m_T[i][j][0][t - 1]) / ap6;
                    }
                }
            }
            // volumes de canto North-West, volume de borda North, volume de canto North-East
            if (i == m_numberOfRows - 1)
            {
                for (size_t j = 0; j < m_numberOfCols; ++j)
                {
                    // canto North-West
                    if (j == 0)
                    {
                        m_T[i][j][it][t] = (aE * m_T[i][j + 1][it - 1][t] + aS * m_T[i - 1][j][it - 1][t] + ap0 * m_T[i][j][0][t - 1]) / ap7;
                    }
                    // borda North
                    if (j > 0 && j < m_numberOfCols - 1)
                    {
                        m_T[i][j][it][t] = (aE * m_T[i][j + 1][it - 1][t] + aW * m_T[i][j - 1][it - 1][t] + aS * m_T[i - 1][j][it - 1][t] + ap0 * m_T[i][j][0][t - 1]) / ap8;
                    }
                    // canto North-East
                    if (j == m_numberOfCols - 1)
                    {
                        m_T[i][j][it][t] = (aW * m_T[i][j - 1][it - 1][t] + aS * m_T[i - 1][j][it - 1][t] + ap0 * m_T[i][j][0][t - 1]) / ap9;
                    }
                }
            }
        }

        // Se o processo iterativo, baseado no numero de iterações definida, termina, o grid recebe os valores calculados no final das iterações
        if (it == m_numberOfIterations - 1)
        {

            for (size_t i = 0, iAux = m_numberOfRows - 1; i < m_numberOfRows; ++i, --iAux)
            {
                for (size_t j = 0; j < m_numberOfCols; ++j)
                {
                    //m_initialGrid->setData(iAux, j, static_cast<float>(m_T[i][j][it][t] + m_upLift));
                    m_initialGrid->setData(iAux, j, static_cast<float>(m_T[i][j][it][t]));
                }
            }
        }

    }
    timeStepCount++; //Acho que não está sendo usado
}


void DifusionAlgorithmService::executeWithVariableBoundary(
        size_t eastBoundaryFactor = 0,
        size_t westBoundaryFactor = 0,
        size_t southBoundaryFactor = 0,
        size_t northBoundaryFactor = 0 )
{
    size_t t = 1;

	double aE = m_diffusivity * m_deltaY / m_deltaX;
	double aW = m_diffusivity * m_deltaY / m_deltaX;
	double aS = m_diffusivity * m_deltaY / m_deltaX;
	double aN = m_diffusivity * m_deltaY / m_deltaX;

	//esses valores estão sendo inicializados, nos loop iterativo eles vão sendo atualizado com os valores dos vizinhos de borda
	float valueAtBoarderSouth = 0;
	float valueAtBoarderNorth = 0;
	float valueAtBoarderWest = 0;
	float valueAtBoarderEast = 0;

	// estes fatores controlam a condição de contorno: 0 para condição de isolamento, 2 para condição de variável prescrita
	double aEB = eastBoundaryFactor * m_diffusivity * m_deltaY / m_deltaX;
	double aWB = westBoundaryFactor * m_diffusivity * m_deltaY / m_deltaX;
	double aSB = southBoundaryFactor * m_diffusivity * m_deltaX / m_deltaY;
	double aNB = northBoundaryFactor * m_diffusivity * m_deltaX / m_deltaY;

	double ap0 = m_deltaY * m_deltaX / m_deltaT;
	double ap1 = aE + aWB + aN + aSB + ap0;
	double ap2 = aE + aW + aN + aSB + ap0;
	double ap3 = aEB + aW + aN + aSB + ap0;
	double ap4 = aE + aWB + aN + aS + ap0;
	double ap5 = aE + aW + aN + aS + ap0;
	double ap6 = aEB + aW + aN + aS + ap0;
	double ap7 = aE + aWB + aNB + aS + ap0;
	double ap8 = aE + aW + aNB + aS + ap0;
	double ap9 = aEB + aW + aNB + aS + ap0;

	/// ****************************************************************
	///                     GENERICO PARA AS BORDA
	/// ****************************************************************

	for (size_t i = 0, iAux = m_numberOfRows - 1; i < m_numberOfRows; ++i, --iAux)
	{
		for (size_t j = 0; j < m_numberOfCols; ++j)
		{
			m_T[i][j][0][0] = m_initialGrid->getData(iAux, j);
		}
	}

	// size_t timeStepCount = 0; Acho que não está sendo usado

	for (size_t i = 0; i < m_numberOfRows; ++i)
	{
		for (size_t j = 0; j < m_numberOfCols; ++j)
		{
			m_T[i][j][0][1] = m_T[i][j][0][0];
		}
	}

	for (size_t it = 1; it < m_numberOfIterations; ++it)
	{
		for (size_t i = 1; i < m_numberOfRows - 1; ++i)
		{
			if (i == 1)
			{
				for (size_t j = 1; j < m_numberOfCols - 1; ++j)
				{
					valueAtBoarderSouth = m_T[i - 1][j][0][0];
					valueAtBoarderEast = m_T[i][j + 1][0][0];
					valueAtBoarderWest = m_T[i][j - 1][0][0];

					if (j == 1)
					{
						m_T[i][j][it][t] = (aE * m_T[i][j + 1][it - 1][t] + aN * m_T[i + 1][j][it - 1][t] + aSB * valueAtBoarderSouth + aWB * valueAtBoarderWest + ap0 * m_T[i][j][0][t - 1]) / ap1;
					}
					if (j > 1 && j < m_numberOfCols - 2)
					{
						m_T[i][j][it][t] = (aW * m_T[i][j - 1][it - 1][t] + aE * m_T[i][j + 1][it - 1][t] + aN * m_T[i + 1][j][it - 1][t] + aSB * valueAtBoarderSouth + ap0 * m_T[i][j][0][t - 1]) / ap2;
					}
					if (j == m_numberOfCols - 2)
					{

						m_T[i][j][it][t] = (aW * m_T[i][j - 1][it - 1][t] + aEB * valueAtBoarderEast + aN * m_T[i + 1][j][it - 1][t] + aSB * valueAtBoarderSouth + ap0 * m_T[i][j][0][t - 1]) / ap3;
					}
				}
			}

			if (i > 1 && i < m_numberOfRows - 2)
			{
				for (size_t j = 1; j < m_numberOfCols - 1; ++j)
				{
					valueAtBoarderEast = m_T[i][j + 1][0][0];
					valueAtBoarderWest = m_T[i][j - 1][0][0];

					if (j == 1)
					{
						m_T[i][j][it][t] = (aWB * valueAtBoarderWest + aE * m_T[i][j + 1][it - 1][t] + aN * m_T[i + 1][j][it - 1][t] + aS * m_T[i - 1][j][it - 1][t] + ap0 * m_T[i][j][0][t - 1]) / ap4;
					}
					if (j > 1 && j < m_numberOfCols - 2)
					{
						m_T[i][j][it][t] = (aE * m_T[i][j + 1][it - 1][t] + aW * m_T[i][j - 1][it - 1][t] + aN * m_T[i + 1][j][it - 1][t] + aS * m_T[i - 1][j][it - 1][t] + ap0 * m_T[i][j][0][t - 1]) / ap5;
					}
					if (j == m_numberOfCols - 2)
					{
						m_T[i][j][it][t] = (aW * m_T[i][j - 1][it - 1][t] + aEB * valueAtBoarderEast + aN * m_T[i + 1][j][it - 1][t] + aS * m_T[i - 1][j][it - 1][t] + ap0 * m_T[i][j][0][t - 1]) / ap6;
					}
				}
			}

			if (i == m_numberOfRows - 2)
			{
				for (size_t j = 1; j < m_numberOfCols - 1; ++j)
				{
					valueAtBoarderNorth = m_T[i + 1][j][0][0];
					valueAtBoarderEast = m_T[i][j + 1][0][0];
					valueAtBoarderWest = m_T[i][j - 1][0][0];

					if (j == 1)
					{
						m_T[i][j][it][t] = (aWB * valueAtBoarderWest + aE * m_T[i][j + 1][it - 1][t] + aNB * valueAtBoarderNorth + aS * m_T[i - 1][j][it - 1][t] + ap0 * m_T[i][j][0][t - 1]) / ap7;
					}
					if (j > 1 && j < m_numberOfCols - 2)
					{
						m_T[i][j][it][t] = (aE * m_T[i][j + 1][it - 1][t] + aW * m_T[i][j - 1][it - 1][t] + aS * m_T[i - 1][j][it - 1][t] + aNB * valueAtBoarderNorth + ap0 * m_T[i][j][0][t - 1]) / ap8;
					}
					if (j == m_numberOfCols - 2)
					{
						m_T[i][j][it][t] = (aW * m_T[i][j - 1][it - 1][t] + aEB * valueAtBoarderEast + aNB * valueAtBoarderNorth + aS * m_T[i - 1][j][it - 1][t] + ap0 * m_T[i][j][0][t - 1]) / ap9;
					}
				}
			}
		}

        //ofstream myfile;
        //myfile.open("saidaas.txt");
		if (it == m_numberOfIterations - 1)
		{
			for (size_t i = 0, iAux = m_numberOfRows - 1; i < m_numberOfRows; ++i, --iAux)
			{
				for (size_t j = 0; j < m_numberOfCols; ++j)
				{
					// resolvendo o problemasdos zeros nas bordas
					if (i == 0 || i == m_numberOfRows - 1 ||j == 0 || j == m_numberOfCols -1)
					{ 
						m_T[i][j][it][t] = m_T[i][j][0][0];
					}

                    //myfile <<setprecision(12) <<m_T[i][j][it][t] << " ";
					m_initialGrid->setData(iAux, j, static_cast<float>(m_T[i][j][it][t]));

					m_T[i][j][0][t] = m_T[i][j][it][t];
					m_T[i][j][0][t - 1] = m_T[i][j][0][t];
				}
                //myfile << "\n";
			}
		}
	}

}

double DifusionAlgorithmService::getDiffusivity() const
{
    return m_diffusivity;
}

}
}
}
