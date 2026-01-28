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
    int level = 1;
    // Constructeurs et méthodes

    Player() : score(0), name("Player"), id(0)
    {
    }

    ~Player() {}

    void generateTetro(Tetro &tempTetro);
    int hasMadeMove(sf::Event &event);
    void updateScore(int linesCleared);
    void drawDetails(sf::RenderWindow &window, sf::Font &font, int modePlayed, int playerID);
    void initializeSinglePlayer();
    void initializeSplitScreenPlayer(int id);
};

#endif