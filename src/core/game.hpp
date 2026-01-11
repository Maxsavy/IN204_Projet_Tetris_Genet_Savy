#include <SFML/Graphics.hpp>
#include 
#include <list>
#include <random>
#include <string>

#define BOX_SIZE 20

namespace game
{
    /*
    GameController is the central interface of the game.
    This class will monitor the working of game:
    *	Movement of the pieces
    *	Keyboard interactions
    *	Lines disapearring...
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

    /*==================================================*
     *			The main game controller *
     *===================================================*/
    class GameController
    {
    public:
        GameController(sf::RenderWindow &w);
        G

        void start();

    private:
        void gameLoop();
        void setupScene();
        void gameOver(); // no definition yet
        void loadResources();
        void loadTexture(sf::Texture &texture, const std::string &file, sf::Sprite &sprite);
        void setUpBackground(sf::Texture &texture, sf::Sprite &sprite);

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
        float scale;
        int score;

    }; // class game

} // namespace game