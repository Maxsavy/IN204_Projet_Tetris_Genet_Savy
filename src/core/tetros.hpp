#ifndef TETROS_HPP
#define TETROS_HPP

#include <vector>
#include <SFML/Graphics.hpp>

enum class TetroType
{
    I, J, L, O, S, T, Z
};

class Tetro
{
public:
    TetroType type;
    std::vector<sf::Vector2i> blocks; // Positions of blocks relative to pivot
    sf::Vector2i pivot;                // Pivot point for rotation          
    sf::Color color;
    Tetro(TetroType t);
    void rotate_clockwise();
    void rotate_counterclockwise();
};
#endif // TETROS_HPP
