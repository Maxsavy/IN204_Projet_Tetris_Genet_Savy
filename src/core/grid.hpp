#ifndef GRID_HPP
#define GRID_HPP

#include <vector>
#include <SFML/Graphics.hpp>
#include "tetros.hpp"

class Grid {

public:
    int rows;
    int columns;
    int cellSize;

    std::vector<int> cells;
    // Tetro currentTetro;

    Grid(): rows(20), columns(10), cellSize(8) 
    {
        cells = std::vector<int>(rows * columns);
        for (int i = 0; i < rows; i++){
            for (int j = 0; j < columns; j++){
                cells[i * columns + j] = 0;
            }
        }
    }

    void display_terminal() const;

    ~Grid() {}

};

#endif // GRID_HPP