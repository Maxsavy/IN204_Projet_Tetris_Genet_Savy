#ifndef GRID_HPP
#define GRID_HPP

#include <vector>
#include "tetros.hpp"
#include "pieces.hpp"
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

    Grid() : rows(22), columns(10), cellSize(8)
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
    int check_collision(const Tetro &tetro, const std::array<std::array<int, 4>, 4> &shape, int futureX, int FutureY) const;
    void update_with_tetro(const Tetro &tetro, int state);
    void delete_full_rows();
    void drawGrid(sf::RenderWindow &window, const Tetro &currentTetro, const int rows, const int cols, float offsetX = 0.f, float offsetY = 0.f);
    void drawNextGrid(sf::RenderWindow &window, Tetro &nextTetro);

    ~Grid()
    {
    }
};
#endif // GRID_HPP
