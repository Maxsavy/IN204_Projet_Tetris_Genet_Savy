#include "grid.hpp"
#include <iostream>

void Grid::display_terminal() const {
        for (int i = 0; i < rows; i++){
            for (int j = 0; j < columns; j++){
                std::cout << cells[i * columns + j] << " ";
            }
            std::cout << std::endl;
        }
    }