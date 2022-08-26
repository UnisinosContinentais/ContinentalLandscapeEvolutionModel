/*
* Developed by UNISINOS - Projeto Continentais and Petrobras
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: March, 2020
*/
#ifndef CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_DOMAIN_RASTER_H
#define CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_DOMAIN_RASTER_H

#include <algorithm>
#include <vector>
#include <assert.h>

namespace continental {
namespace landscapeevolutionmodel {
namespace domain {

/**
 * @brief Classe do tipo template de definição do Raster.
 * Esta classe contém os dados de um raster.
 */
template <class T>
class Raster
{
public:
    /// Construtor.
    Raster()
        : m_maxValue(std::numeric_limits<T>::min())
    {
        static_assert(std::is_same<T, short>::value || std::is_same<T, float>::value || std::is_same<T, double>::value, "Raster only accepts short, float or double.");
    }

    /// Construtor.
    /// @param rows Número de linhas do raster.
    /// @param cols Número de colunas do raster.
    /// @param xOrigin Valor da origem em X.
    /// @param yOrigin Valor da origem em Y.
    /// @param cellSize Valor do tamanho da célula.
    /// @param noData Valor do Nodata.
    Raster(size_t rows, size_t cols, double xOrigin, double yOrigin, double cellSize, int noData)
        : Raster()
    {
        reset(rows, cols, xOrigin, yOrigin, cellSize, noData);
    }

    /// Função de retorno do número de linhas.
    /// @return Valor do número de linhas.
    inline size_t getRows() const
    {
        return m_rows;
    }

    /// Função de retorno do número de colunas.
    /// @return Valor do número de colunas.
    inline size_t getCols() const
    {
        return m_cols;
    }

    /// Função de retorno da resolução da grade.
    /// @return Valor da resolução da grade.
    inline double getCellSize() const
    {
        return m_cellSize;
    }

    /// Função de retorno do valor para o qual não se tem dados no raster.
    /// @return Valor para o qual não se tem dados no raster.
    inline T getNoDataValue() const
    {
        return m_noDataValue;
    }

    /// Função de retorno do número total de células.
    /// @return Valor do número total de células.
    inline size_t getTotalCells() const
    {
        return m_cols * m_rows;
    }

    /// Função de retorno da coordenada X do canto superior da grade.
    /// @return Valor da coordenada X do canto superior da grade.
    inline double getXOrigin() const
    {
        return m_x_origin;
    }

    /// Função de retorno da coordenada Y do canto superior da grade.
    /// @return Valor da coordenada Y do canto superior da grade.
    inline double getYOrigin() const
    {
        return m_y_origin;
    }

    /// Função de retorno do máximo valor dos dados da matriz(y, x) do raster.
    /// @return Valor máximo dos dados da matriz(y, x) do raster.
    inline T calculateMaxValue() const
    {
        auto maxValue = m_data[0];
        for (auto item : m_data)
        {
            maxValue = std::max(maxValue, item);
        }
        return maxValue;
    }

    /// Função de retorno do valor de uma célula.
    /// @param position Posição da célula.
    /// @return Valor da célula.
    inline T getData(const size_t position) const
    {
        assert(position < m_data.size());

        return m_data[position];
    }

    /// Função de retorno do valor de uma célula.
    /// @param row Número da linha.
    /// @param column Número da coluna.
    /// @return Valor da célula.
    inline T getData(const size_t row, const size_t column) const
    {
        return getData(row * m_cols + column);
    }

    /// Função de retorno do valor de uma célula.
    /// @param row Número da linha.
    /// @param column Número da coluna.
    /// @return Valor da célula.
    inline T getData(const int row, const int column) const
    {
        return getData(static_cast<size_t>(row), static_cast<size_t>(column));
    }

    /// Função de definição do valor de uma célula.
    /// @param position Posição da célula.
    /// @param value Valor da célula.
    inline void setData(const size_t position, const T value)
    {
        assert(position < m_data.size());

        m_data[position] = value;
    }

    /// Função de definição do valor de uma célula.
    /// @param row Número da linha.
    /// @param column Número da coluna.
    /// @param value Valor da célula.
    inline void setData(const size_t row, const size_t column, const T value)
    {
        setData(row * m_cols + column, value);
    }

    /// Função de retorno do vetor de valores das células.
    /// @return Vetor de valores das células.
    inline const std::vector<T>& getData() const
    {
        return m_data;
    }

    /// Função de reset do raster.
    /// @param rows Número de linhas do raster.
    /// @param cols Número de colunas do raster.
    /// @param xOrigin Valor da origem em X.
    /// @param yOrigin Valor da origem em Y.
    /// @param cellSize Valor do tamanho da célula.
    /// @param noData Valor do Nodata.
    inline void reset(size_t rows, size_t cols, double xOrigin, double yOrigin, double cellSize, int noData)
    {
        m_cols = cols;
        m_rows = rows;
        m_x_origin = xOrigin;
        m_y_origin = yOrigin;
        m_cellSize = cellSize;
        m_noDataValue = noData;
        m_data.clear();
        m_data.resize(rows * cols);
    }

    /// Função de retorno do status da modificação do raster.
    /// @return Retorna um status se o raster foi modificada.
    inline bool getModified() const
    {
        return m_modified;
    }

    /// Função que define o status da modificação do raster.
    /// @param modified Status de modificação do raster.
    inline void setModified(bool modified)
    {
        m_modified = modified;
    }

    /// Função de retorno do status da persistência do raster.
    /// @return Retorna um status se o raster esta persistente.
    inline bool isPersisted() const
    {
        return m_hasPersisted;
    }

    /// Função que define o status da persistência do raster.
    /// @param hasPersisted Status de persistência do raster.
    inline void setPersisted(bool hasPersisted)
    {
        m_hasPersisted = hasPersisted;
    }

    /// Função de retorno do valor da versão.
    /// @return Valor da versão.
    inline unsigned short getVersion() const
    {
        return m_version;
    }

    /// Função de definição do valor da versão.
    /// @param version Valor da versão.
    inline void setVersion(unsigned short version)
    {
        m_version = version;
    }

    /// Função referente se o dados é NoData
    /// \param row Valor da linha
    /// \param col Valor da Coluna
    /// \return Retorna os dados
    inline bool isNoDataValue(size_t row, size_t col)
    {
        return static_cast<int>(getData(row, col)) == static_cast<int>(m_noDataValue);
    }

private:
    /// Membros.
    size_t m_cols = 0; // Numero Colunas
    size_t m_rows = 0; // Numero Linhas
    double m_x_origin = 0;
    double m_y_origin = 0;
    double m_cellSize = 0; // Resolução do raster
    T m_noDataValue = 0; // Valor do NODATA
    bool m_modified = true;
    bool m_hasPersisted = false;
    unsigned short m_version = 0;
    std::vector<T> m_data;
    T m_maxValue;
};

} //domain
} //landscapeevolutionmodel {
} //continental

#endif // CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_DOMAIN_RASTER_H
