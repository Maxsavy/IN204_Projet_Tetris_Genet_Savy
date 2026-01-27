#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include "grid.hpp"
#include "tetros.hpp"

class Player
{
public:
    // Les attributs d'un joueur

    std::string name;
    unsigned int id;
    int score = 0;
    Tetro currentTetro;
    Tetro nextTetro;
    std::vector<TetroType> tetroList;
    Grid playerGrid;
    Grid nextTetroGrid;

    // Constructeurs et méthodes

    Player() : score(0), name("Player"), id(0)
    {
    }

    ~Player() {}

    void generateTetro(Tetro &tempTetro);
    void lockTetroInGrid(const Tetro &tetro);
    void updateScore(int linesCleared);
    void drawScore(sf::RenderWindow &window, sf::Font &font);
};

#endif