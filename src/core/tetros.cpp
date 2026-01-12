#ifndef TETROS_CPP
#define TETROS_CPP

#include <vector>
#include "tetros.hpp"
#include "pieces.hpp"
#include <SFML/Graphics.hpp>

void Tetro::initializeShape(const int (&piece)[4][4][4])
{
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

Tetro::Tetro() : position(0, 0), currentRotation(0)
{
    // base constructor does not initialize a specific piece
}

TetroI::TetroI() : Tetro()
{
    color = sf::Color::Cyan;
    initializeShape(I_PIECE);
}

TetroJ::TetroJ() : Tetro()
{
    color = sf::Color::Blue;
    initializeShape(J_PIECE);
}

TetroL::TetroL() : Tetro()
{
    color = sf::Color(255, 165, 0); // Orange
    initializeShape(L_PIECE);
}

TetroO::TetroO() : Tetro()
{
    color = sf::Color::Yellow;
    initializeShape(O_PIECE);
}

TetroS::TetroS() : Tetro()
{
    color = sf::Color::Green;
    initializeShape(S_PIECE);
}

TetroT::TetroT() : Tetro()
{
    color = sf::Color::Magenta;
    initializeShape(T_PIECE);
}

TetroZ::TetroZ() : Tetro()
{
    color = sf::Color::Red;
    initializeShape(Z_PIECE);
}

void Tetro::rotate()
{
    currentRotation = (currentRotation + 1) % 4;
}

void Tetro::moveDown()
{
    position.y += 1;
}

void Tetro::moveLeft()
{
    position.x -= 1;
}

void Tetro::moveRight()
{
    position.x += 1;
}


const std::array<std::array<int, 4>, 4> &Tetro::getShape() const
{
    return rotations[currentRotation];
}

void Tetro::setPosition(int gridX, int gridY)
{
    position.x = gridX;
    position.y = gridY;
}

#endif // TETROS_CPP