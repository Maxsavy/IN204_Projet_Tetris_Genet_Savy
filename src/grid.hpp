#ifndef GRID_HPP
#define GRID_HPP

#include <array>
#include <SFML/Graphics.hpp>

class Grid {

public:
    int rows;
    int columns;
    int cellSize;

    std::array<int> cells;

    Grid(): rows(20), columns(10), cellSize(8) 
    {
        cells = std::array<int>(rows * columns);
        for (int i = 0; i < rows: i++){
            for (int j = 0; j < columns; j++){
                cells[i * columns + j] = 0;
            }
        }
    }

    ~Grid() {}


#endif // GRID_HPP