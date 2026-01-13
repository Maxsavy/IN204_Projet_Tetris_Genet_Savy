#ifndef TETROS_HPP
#define TETROS_HPP

#include <array>
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
    virtual ~Tetro() = default;

    sf::Vector2i position;
    int currentRotation;
    sf::Color color;
    int colorRef;

    void rotate();
    void moveDown();
    void moveLeft();
    void moveRight();
    const std::array<std::array<int, 4>, 4> &getShape() const;
    void setPosition(int x, int y);

protected:
    void initializeShape(const int (&piece)[4][4][4]);

private:
    std::array<std::array<std::array<int, 4>, 4>, 4> rotations;
};

class TetroI : public Tetro
{
public:
    TetroI();
};

class TetroJ : public Tetro
{
public:
    TetroJ();
};

class TetroL : public Tetro
{
public:
    TetroL();
};

class TetroO : public Tetro
{
public:
    TetroO();
};

class TetroS : public Tetro
{
public:
    TetroS();
};

class TetroT : public Tetro
{
public:
    TetroT();
};

class TetroZ : public Tetro
{
public:
    TetroZ();
};
#endif // TETROS_HPP
