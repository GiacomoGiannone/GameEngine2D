#include "Level.hpp"
#include <algorithm>
#include <stdexcept>

namespace GE
{
    void Level::addRow(const std::string& row)
    {
        rows.push_back(row);
        height = rows.size();
        if (!row.empty())
        {
            width = std::max(width, static_cast<int>(row.size()));
        }
    }

    int Level::getNumRows() const
    {
        return height;
    }

    int Level::getNumCols() const
    {
        return width;
    }

    const std::string& Level::getRow(int row) const
    {
        if (row < 0 || row >= height)
        {
            throw std::out_of_range("Row index out of range");
        }

        return rows[row];
    }

    char Level::getCell(int row, int col) const
    {
        if (row < 0 || row >= static_cast<int>(rows.size()) || col < 0 || col >= static_cast<int>(rows[row].size()))
        {
            throw std::out_of_range("Row or column index out of range");
        }
        return rows[row][col];
    }
}