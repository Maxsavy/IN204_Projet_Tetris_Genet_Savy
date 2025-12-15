#ifndef PIECES_HPP
#define PIECES_HPP

#include "grille.hpp"

#include <array>

using namespace std;

class BasePiece
{
    private:
        int color;
        array<Case,4> surface;

    public:
        BasePiece() {}

        ~BasePiece() {}

    void setColor(int c);

    virtual void const rotateRight();

    virtual void const moveRight();

    virtual void const moveLeft();

    virtual void const moveDown();
};

class SquarePiece: virtual public BasePiece
{
    private:
        int color;
        array<Case,4> surface;

    public:
        SquarePiece() {}

        ~BasePiece() {}

    virtual void const rotateRight();

    virtual void const moveRight();

    virtual void const moveLeft();

    virtual void const moveDown();
};

#endif