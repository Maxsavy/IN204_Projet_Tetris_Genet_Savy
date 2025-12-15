#include <iostream>
#include "grille.hpp"
#include "pieces.hpp"


using namespace std;


int main()
{
    const int h = 20;
    const int w = 10;
    Grille G(h,w);
    G.initSize();
    G.printGrille();
    return EXIT_SUCCESS;
}