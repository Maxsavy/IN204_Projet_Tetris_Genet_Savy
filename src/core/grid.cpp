#include "grid.hpp"
#include "tetros.hpp"
#include <iostream>

void Grid::display_terminal() const
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            std::cout << cells[i * columns + j] << " ";
        }
        std::cout << std::endl;
    }
}

bool Grid::check_collision(const Tetro &tetro, int futureX, int futureY) const
{
    const auto &shape = tetro.getShape();

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (shape[i][j] == 1)
            {
                int cibleX = futureX + j;
                int cibleY = futureY + i;

                // Check boundaries
                if (cibleX < 0 || cibleX >= columns || cibleY >= rows)
                {
                    return true; // Collision with walls or floor
                }

                
            }   
        }
    }
    return false; // No collision
}

void Grid::update_with_tetro(const Tetro &tetro)
{
    for (int i = 0; i < rows * columns; i++)
    {
        if (cells[i] == 2)
            cells[i] = 0;
    }

    const auto &shape = tetro.getShape();

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (shape[i][j] == 1)
            {
                int gridX = tetro.position.x + j;
                int gridY = tetro.position.y + i;

                if (gridX >= 0 && gridX < columns && gridY >= 0 && gridY < rows)
                {
                    int idx = gridY * columns + gridX;
                    cells[idx] = 2;
                }
            }
        }
    }
}

void Grid::drawGrid(sf::RenderWindow &window)
{
    this->update_with_tetro(this->tempTetro);
    int rows = this->rows;
    int cols = this->columns;
    int cellSize = this->cellSize;
    float pixelSize = static_cast<float>(cellSize * RESIZE_FACTOR);
    float targetW = static_cast<float>(cols) * pixelSize;
    float targetH = static_cast<float>(rows) * pixelSize;
    float originX = (static_cast<float>(window.getSize().x) - targetW) / 2.f;
    float originY = (static_cast<float>(window.getSize().y) - targetH) / 2.f;

    sf::RectangleShape cellShape(sf::Vector2f(pixelSize, pixelSize));
    cellShape.setOutlineThickness(1.f);
    cellShape.setOutlineColor(sf::Color(100, 100, 100));

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            float x = originX + j * pixelSize;
            float y = originY + i * pixelSize;
            cellShape.setPosition(x, y);
            int idx = j + i * cols;
            int val = 0;
            if (idx >= 0 && idx < static_cast<int>(this->cells.size()))
                val = this->cells[idx];

            if (val == 0)
                cellShape.setFillColor(sf::Color::Black);
            else if (val == 1)
                cellShape.setFillColor(sf::Color::White); // locked pieces
            else if (val == 2)
                cellShape.setFillColor(this->tempTetro.color); // active piece with its color

            window.draw(cellShape);
        }
    }
}