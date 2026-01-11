#ifndef TETROS_CPP
#define TETROS_CPP

#include <vector>
#include "tetros.hpp"
#include "rotations.hpp"
#include <SFML/Graphics.hpp>

void Tetro::initializeShape()
{
    const int (*piece)[4][4] = nullptr;

    switch (type)
    {
    case TetroType::I:
        piece = I_PIECE;
        break;
    case TetroType::O:
        piece = O_PIECE;
        break;
    case TetroType::T:
        piece = T_PIECE;
        break;
    case TetroType::S:
        piece = S_PIECE;
        break;
    case TetroType::Z:
        piece = Z_PIECE;
        break;
    case TetroType::J:
        piece = J_PIECE;
        break;
    case TetroType::L:
        piece = L_PIECE;
        break;
    }

    // Copy to rotations array
    for (int r = 0; r < 4; r++)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                rotations[r][i][j] = piece[r][i][j];
            }
        }
    }
}

#endif // TETROS_CPP