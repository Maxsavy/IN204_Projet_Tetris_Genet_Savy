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

void Grid::update_with_tetro(const Tetro &tetro)
{
    // Reset grid to only show locked pieces (assuming 1 = locked, 2 = active piece)
    for (int i = 0; i < rows * columns; i++)
    {
        if (cells[i] == 2) // clear previous active piece position
            cells[i] = 0;
    }

    // Get the current shape
    const auto &shape = tetro.getShape();

    // Draw the tetro on the grid
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (shape[i][j] == 1)
            {
                // Calculate grid position
                int gridX = tetro.position.x + j;
                int gridY = tetro.position.y + i;

                // Check bounds
                if (gridX >= 0 && gridX < columns && gridY >= 0 && gridY < rows)
                {
                    int idx = gridY * columns + gridX;
                    cells[idx] = 2; // 2 = active piece (vs 1 = locked)
                }
            }
        }
    }
}