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

int Grid::check_collision(const Tetro &tetro, int futureX, int futureY) const
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
                if (cibleX < 0 || cibleX >= columns)
                {
                    return 1; // Collision with walls or floor
                }

                if (cibleY >= rows)
                {
                    return 2; // Collision with floor
                }

                int indx = cibleY * columns + cibleX;
                if (cells[indx] == 1)
                {
                    return 1; // Collision with locked pieces
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
                    cells[idx] = state;
                }
            }
        }
    }
}

void Grid::drawGrid(sf::RenderWindow &window)
{
    this->update_with_tetro(this->tempTetro, 2);
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

void Grid::generateTetro()
{
    if (tetroList.empty())
    {
        tetroList = std::vector<TetroType>(TETRO_BAG.begin(), TETRO_BAG.end());
    }
    int randomIndex = rand() % tetroList.size();
    TetroType selectedType = tetroList[randomIndex];
    tetroList.erase(tetroList.begin() + randomIndex);

    switch (selectedType)
    {
    case TetroType::I:
        tempTetro = TetroI();
        break;
    case TetroType::J:
        tempTetro = TetroJ();
        break;
    case TetroType::L:
        tempTetro = TetroL();
        break;
    case TetroType::O:
        tempTetro = TetroO();
        break;
    case TetroType::S:
        tempTetro = TetroS();
        break;
    case TetroType::T:
        tempTetro = TetroT();
        break;
    case TetroType::Z:
        tempTetro = TetroZ();
        break;
    default:
        tempTetro = TetroI();
        break;
    }

    tempTetro.setPosition(columns / 2 - 2, 0);
}

void Grid::lockTetroInGrid(const Tetro &tetro)
{
    update_with_tetro(tetro, 1);
    generateTetro();
}