#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include "grid.hpp"
#include "tetros.hpp"
#include <SFML/Audio.hpp>
#include <iostream>

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
    sf::Music moveSound;
    // Constructeurs et méthodes

    Player() : score(0), name("Player"), id(0)
    {
        if (!moveSound.openFromFile("assets/sounds/move_rotation.mp3"))
        {
            std::cerr << "Failed to load music file" << std::endl;
        }
        else
        {
            moveSound.setVolume(100);
        }
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