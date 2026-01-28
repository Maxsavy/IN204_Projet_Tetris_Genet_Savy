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

private:
    sf::Clock lockingDelayClock;
    bool locking = false;

public:
    int rows;
    int columns;
    int cellSize;
    int linesCleared = 0;
    int totalLinesCleared = 0;
    float originX = 0.0f;
    float originY = 0.0f;
    int playerId = 0;
    int gameMode = 1;
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
    std::vector<int> delete_full_rows();
    void draw_deleted_row_animation(sf::RenderWindow &window, const std::vector<int> &deletedRow);
    void setGridPosition(int mode, int pId, float windowWidth, float windowHeight);
    void drawGrid(sf::RenderWindow &window, const Tetro &currentTetro, const int rows, const int cols, float originX = 0.f, float originY = 0.f, float pixelSize = CELL_SIZE * RESIZE_FACTOR);
    void drawGameGrid(sf::RenderWindow &window, const Tetro &currentTetro);
    void drawNextGrid(sf::RenderWindow &window, Tetro &nextTetro);
    void start_locking_timer();
    void cancel_locking_timer();
    bool locking_tetro(const Tetro &tetro, int move_count);
    int check_collision(const Tetro &tetro, const std::array<std::array<int, 4>, 4> &shape, int futureX, int FutureY);
    std::pair<int, int> try_wall_kicks(const Tetro &tetro, const std::array<std::array<int, 4>, 4> &newShape, int currentRotation);
    void update_with_ghost_tetro(const Tetro &tetro, int state);
    void drawGhostTetro(sf::RenderWindow &window, const Tetro &currentTetro);

    ~Grid()
    {
    }
};
#endif // GRID_HPP
