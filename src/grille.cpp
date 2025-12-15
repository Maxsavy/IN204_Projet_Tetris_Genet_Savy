#include "grille.hpp"
#include <string>
using namespace std;

/* Définition de méthodes pour la classe Case*/

int Case::getX() const {return m_coordX;}

int Case::getY()const {return m_coordY;}

int Case::getState()const {return state;}

void Case::setCoord(int x, int y)
{
    m_coordX = x;
    m_coordY = y;
}

/* Définition de méthodes pour la classe Grille*/

void Grille::initSize()
{
    int size = height*wight;
    m_grille.assign(size,Case(0));
    for(int i=0;i<m_grille.size();i++)
    {
        m_grille[i].setCoord(i%wight+1, i/wight + 1);
    }
}

void Grille::printGrille()
{
    for (int i=0;i<wight;i++){ cout << "##";}
    cout << endl;
    for(int i=0;i<m_grille.size();i++)
    {
        int etat = m_grille[i].getState();
        cout << etat <<" ";
        if ((i+1)%wight == 0){cout << endl;}  
    }
    for (int i=0;i<wight;i++){ cout << "##";}
    cout <<endl;
}