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

void Grid::update_with_tetro(Tetro &tetro) {
    for (size_t i = 0; i < tetro.shape.size(); i++) {
            if (tetro.shape[i] != 0) {
                int grid_x = tetro.x + i%2;
                int grid_y = tetro.y + i/2;
                if (grid_x >= 0 && grid_x < columns && grid_y >= 0 && grid_y < rows) {
                    cells[grid_y * columns + grid_x] = tetro.shape[i];
                }
                if (grid_y >= rows) {
                    tetro.reset();
                }
            }
    }

    for (int i = 0; i < columns; i++){
        cells[i+(tetro.y -1) * columns] = 0;
    }
    
    
}   