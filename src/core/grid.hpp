#ifndef GRID_HPP
#define GRID_HPP

#include <vector>
#include "tetros.hpp"
#include <SFML/Graphics.hpp>

constexpr int ROWS = 20;
constexpr int COLUMNS = 10;
constexpr int CELL_SIZE = 8;
constexpr int RESIZE_FACTOR = 4;

class Grid
{

public:
    int rows;
    int columns;
    int cellSize;
    std::vector<int> cells;
    // initialize tetro
    TetroJ tempTetro;

    Grid() : rows(20), columns(10), cellSize(8)
    {
        cells = std::vector<int>(rows * columns);
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                cells[i * columns + j] = 0;
            }
        }
    }

    void display_terminal() const;
    void update_with_tetro(const Tetro &tetro);
    void drawGrid(sf::RenderWindow &window);

    ~Grid()
    {
    }
};
#endif // GRID_HPP
