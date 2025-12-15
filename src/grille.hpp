#ifndef GRILLE_HPP
#define GRILLE_HPP

#include <iostream>
#include <vector>

using namespace std;
class Case
{
    private:
        int m_coordX;
        int m_coordY;
        int state;


    public:


        Case() {}

        Case(int State): state(State) 
        {}

        Case(int X, int Y, int State): m_coordX(X), m_coordY(Y), state(State) 
        {}

        Case(const Case& anotherCase): m_coordX(anotherCase.m_coordX), m_coordY(anotherCase.m_coordY), state(anotherCase.state)
        {}

        ~Case() 
        {
           // cout << "destroyed case" << endl;
        }

        int getX() const;

        int getY() const;

        int getState() const;

        void setCoord(int x, int y);

};

class Grille
{
    private:
        
        int height;
        int wight;
        vector<Case> m_grille;

    public:

        Grille() {}
        Grille(int h, int w): height(h), wight(w)
        {}

        ~Grille() 
        {
            cout << "La grille de taille " << height << " par " << wight << " a ete detruite"<< endl;
        }

        void initSize();

        void printGrille();
};

#endif