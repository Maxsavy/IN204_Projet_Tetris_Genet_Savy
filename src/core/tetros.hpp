#ifndef TETROS_HPP
#define TETROS_HPP

#include <vector>
#include <SFML/Graphics.hpp>

enum class TetroType
{
    I,
    J,
    L,
    O,
    S,
    T,
    Z
};

class Tetro
{
public:
    Tetro();
    Tetro(TetroType type) : type(type) {}
    TetroType type;
    sf::Vector2i position;
    sf::Color color;
    void rotate();
    const std::array<std::array<int, 4>, 4> &getShape() const;

private:
    std::array<std::array<std::array<int, 4>, 4>, 4> rotations;
    void initializeShape();
};

class TetroI : Tetro
{
};

#endif // TETROS_HPP
