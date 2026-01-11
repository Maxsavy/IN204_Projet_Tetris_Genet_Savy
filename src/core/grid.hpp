#ifndef GRID_HPP
#define GRID_HPP

#include <vector>
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
    sf::RectangleShape rectangle;

    std::vector<int> cells;

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
        // initialize rectangle used for rendering cells
        rectangle.setSize(sf::Vector2f(static_cast<float>((cellSize)*RESIZE_FACTOR), static_cast<float>((cellSize)*RESIZE_FACTOR)));
        rectangle.setFillColor(sf::Color::Green);
    }

    void display_terminal() const;

    ~Grid() {}
};
#endif // GRID_HPP
