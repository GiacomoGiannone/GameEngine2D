#pragma once

#include <string>
#include <vector>

namespace GE
{
    class Level
    {
        private:
            std::vector<std::string> rows; // Each string represents a row in the level layout

            int width;
            int height;

            int StartingPointX;
            int StartingPointY;

        public:
            Level() : width(0), height(0), StartingPointX(0), StartingPointY(0) {}

            void addRow(const std::string& row);
            int getNumRows() const;
            int getNumCols() const;
            const std::string& getRow(int row) const;
            //method to get cell at a specific row and column
            char getCell(int row, int col) const;
    };
}