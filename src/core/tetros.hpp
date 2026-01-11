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
    int x;
    int y;
    std::vector<int> shape;

    Tetro(): x(4), y(0), shape({1,1,
                                1,1})
    {}

    ~Tetro() {}

    bool move_down()
    {
        y += 1;
        return true;
    }

    void reset()
    {
        x = 4;
        y = 0;
    }

};

#endif // TETROS_HPP
