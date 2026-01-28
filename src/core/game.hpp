#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "player.hpp"
#include <list>
#include <random>
#include <string>

#define BOX_SIZE 20

namespace game
{
    /*
    GameController is the central interface of the game.
    This class will monitor the working of game:
    *   Movement of the pieces
    *   Keyboard interactions
    *   Lines disapearring...
    */

    const std::string MenuStrings[5] = {"Game Over!", "Do you wish to continue?",
                                        "Yes", "No", "Exit"}; // menu strings

    enum MenuText
    {
        GameOver,
        Continue,
        Yes,
        No,
        Exit,
    }; // menuText

    constexpr int ROWS = 20;
    constexpr int COLUMNS = 10;
    constexpr int CELL_SIZE = 8;
    constexpr int RESIZE_FACTOR = 4;

    /*==================================================*
     *          The main game controller *
     *===================================================*/
    class GameController
    {
    public:
        GameController(sf::RenderWindow &w);
        Player player[2];
        sf::Clock gameClock;
        void gameLoop();
        void start(int mode);

    private:
        void setupScene();
        void gameOver();
        void pause();
        void loadResources();
        void loadTexture(sf::Texture &texture, const std::string &file, sf::Sprite &sprite);
        void setUpBackground(sf::Texture &texture, sf::Sprite &sprite);
        void handlePlayerInput(int playerId, sf::Event &event, int &moveCount, bool &isTouchingGround);

        //    Grid grid;
        sf::Texture background;
        sf::Texture grid;
        sf::Texture logo;
        sf::Sprite background_sprite;
        sf::Sprite grid_sprite;
        sf::Sprite logo_sprite;
        sf::RenderWindow &_window;
        sf::Vector2i direction;
        sf::Font font;
        sf::Text scoreText;
        float scale;
        int score;
        int modePLayed;
        bool isPaused = false;

    }; // class game

} // namespace game

#endif // GAME_HPP