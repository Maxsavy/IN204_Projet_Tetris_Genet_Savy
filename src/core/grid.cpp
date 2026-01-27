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

int Grid::check_collision(const Tetro &tetro, const std::array<std::array<int, 4>, 4> &shape, int futureX, int futureY) const
{
    // fonction de gestion des collisions pour les 4 cas suivantts:
    // 0: pas de collision
    // 1: collision mur gauche/droite ou piece verouillée
    // 2: collision sol ou piece verouillée en descendant
    // 3: collision plafond

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (shape[i][j] == 1)
            {
                int cibleX = futureX + j;
                int cibleY = futureY + i;

                // Check des collisions

                if (cibleX < 0 || cibleX >= columns)
                {
                    return 1; // Collisions avec les murs
                }

                if (cibleY >= rows)
                {
                    return 2; // Collision avec le sol
                }

                int indx_bottom = (tetro.position.y + 1 + i) * columns + cibleX;
                if (cells[indx_bottom] != 0 && cells[indx_bottom] != 1)
                {
                    if (tetro.position.y - 1 < 1)
                        return 3; // Collision avec le plafond
                    return 2;     // Collision avec une pièce lockée en dessous
                }

                int indx = cibleY * columns + cibleX;
                if (cells[indx] != 0 && cells[indx] != 1)
                {
                    return 1; // Collision avec une pièce lockée sur les côtés
                }
            }
        }
    }
    return 0; // No collision
}

void Grid::update_with_tetro(const Tetro &tetro, int state)
{
    for (int i = 0; i < rows * columns; i++)
    {
        if (cells[i] == 1)
            cells[i] = 0;
    }

    const auto &shape = tetro.getShape(0);

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
                    cells[idx] = state;
                }
            }
        }
    }
}

void Grid::delete_full_rows()
{
    for (int i = 0; i < rows; i++)
    {
        bool isFull = true;
        for (int j = 0; j < columns; j++)
        {
            if (cells[i * columns + j] == 0 || cells[i * columns + j] == 1)
            {
                isFull = false;
                break;
            }
        }

        if (isFull)
        {
            linesCleared++;
            // Move all rows above down by one
            for (int k = i; k > 0; k--)
            {
                for (int j = 0; j < columns; j++)
                {
                    cells[k * columns + j] = cells[(k - 1) * columns + j];
                }
            }
            // Clear the top row
            for (int j = 0; j < columns; j++)
            {
                cells[j] = 0;
            }
        }
    }
}

void Grid::drawNextGrid(sf::RenderWindow &window, Tetro &nextTetro)
{

    float pixelSize = static_cast<float>(CELL_SIZE * RESIZE_FACTOR);
    float mainGridW = static_cast<float>(COLUMNS) * pixelSize;
    float mainGridH = static_cast<float>(ROWS - 2) * pixelSize;
    float mainOriginX = (static_cast<float>(window.getSize().x) - mainGridW) / 2.f;
    float mainOriginY = (static_cast<float>(window.getSize().y) - mainGridH) / 2.f;
    float nextGridOffsetX = mainOriginX + mainGridW + 30.f;
    float nextGridOffsetY = mainOriginY;

    nextTetro.setPosition(1, -1);
    this->update_with_tetro(nextTetro, 1);
    drawGrid(window, nextTetro, 6, 6, nextGridOffsetX, nextGridOffsetY, pixelSize);
    nextTetro.setPosition(3, -1);
}

void Grid::drawGameGrid(sf::RenderWindow &window, const Tetro &tetro, int rows, const int cols)
{
    this->update_with_tetro(tetro, 1);
    rows = rows - 2;
    int cellSize = this->cellSize;
    float pixelSize = static_cast<float>(CELL_SIZE * RESIZE_FACTOR);
    float mainGridW = static_cast<float>(cols) * pixelSize;
    float mainGridH = static_cast<float>(rows - 2) * pixelSize;
    float mainOriginX = (static_cast<float>(window.getSize().x) - mainGridW) / 2.f;
    float mainOriginY = (static_cast<float>(window.getSize().y) - mainGridH) / 2.f;

    drawGrid(window, tetro, rows, cols, mainOriginX, mainOriginY, pixelSize);
}

void Grid::drawGrid(sf::RenderWindow &window, const Tetro &tetro, int rows, const int cols, float originX, float originY, float pixelSize)
{
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
            int rowOffset = (cols == 6) ? -2 : 2;
            int idx = j + (i + rowOffset) * this->columns;
            int val = 0;
            if (idx >= 0 && idx < static_cast<int>(this->cells.size()))
                val = this->cells[idx];

            if (val == 0)
                cellShape.setFillColor(sf::Color::Black);
            else if (val == 1)
                cellShape.setFillColor(tetro.color); // active piece with its color
            else if (val != 0 && val != 1)
            {
                switch (val)
                {
                case 2:
                    cellShape.setFillColor(sf::Color::Cyan); // I piece
                    break;
                case 3:
                    cellShape.setFillColor(sf::Color::Blue); // J piece
                    break;
                case 4:
                    cellShape.setFillColor(sf::Color(255, 165, 0)); // L piece
                    break;
                case 5:
                    cellShape.setFillColor(sf::Color::Yellow); // O piece
                    break;
                case 6:
                    cellShape.setFillColor(sf::Color::Green); // S piece
                    break;
                case 7:
                    cellShape.setFillColor(sf::Color::Magenta); // T piece
                    break;
                case 8:
                    cellShape.setFillColor(sf::Color::Red); // Z piece
                    break;
                }
            }

            window.draw(cellShape);
        }
    }
}
