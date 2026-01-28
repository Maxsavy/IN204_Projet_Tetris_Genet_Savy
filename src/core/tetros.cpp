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
    // Initialize the base class
}

TetroI::TetroI() : Tetro()
{
    color = sf::Color::Cyan;
    colorRef = 2;
    initializeShape(I_PIECE);
}

TetroJ::TetroJ() : Tetro()
{
    color = sf::Color::Blue;
    colorRef = 3;
    initializeShape(J_PIECE);
}

TetroL::TetroL() : Tetro()
{
    color = sf::Color(255, 165, 0); // Orange
    colorRef = 4;
    initializeShape(L_PIECE);
}

TetroO::TetroO() : Tetro()
{
    color = sf::Color::Yellow;
    colorRef = 5;
    initializeShape(O_PIECE);
}

TetroS::TetroS() : Tetro()
{
    color = sf::Color::Green;
    colorRef = 6;
    initializeShape(S_PIECE);
}

TetroT::TetroT() : Tetro()
{
    color = sf::Color::Magenta;
    colorRef = 7;
    initializeShape(T_PIECE);
}

TetroZ::TetroZ() : Tetro()
{
    color = sf::Color::Red;
    colorRef = 8;
    initializeShape(Z_PIECE);
}

void Tetro::rotate()
{
    currentRotation = (currentRotation + 1) % 4;
}

void Tetro::moveDown(int i)
{
    position.y += i;
}

void Tetro::moveLeft()
{
    position.x -= 1;
}

void Tetro::moveRight()
{
    position.x += 1;
}


const std::array<std::array<int, 4>, 4> &Tetro::getShape(int i) const
{
    return rotations[(currentRotation+i) % 4];
}

void Tetro::setPosition(int gridX, int gridY)
{
    position.x = gridX;
    position.y = gridY;
}

#endif // TETROS_CPP