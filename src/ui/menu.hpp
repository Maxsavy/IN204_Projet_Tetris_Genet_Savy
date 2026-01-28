#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <game_menu/game_menu.h>
#include "../core/game.hpp"
#include <memory>

namespace game
{
    extern sf::Texture background;
    extern sf::Sprite background_sprite;

    class MainMenu
    {
    public:
        int last_selected_mode = 2;

        MainMenu(sf::RenderWindow &w);
        void start();
        void showGameOverMenu(int finalScore = 0, int finalLevel = 1);
        void showPauseMenu(GameController *gameController);
        void setup_menu_context();

    private:
        
        void setup_game_over_menu();
        void setup_pause_menu();

        sf::RenderWindow &_window;
        sf::Font _font;
        sf::Music _background_music;
        game_menu::MENU *_current_menu;
        std::unique_ptr<game_menu::MENU, std::function<void(game_menu::MENU *)>>
            _main_menu_context;
        std::unique_ptr<game_menu::MENU, std::function<void(game_menu::MENU *)>>
            _game_over_menu_context;
        std::unique_ptr<game_menu::MENU, std::function<void(game_menu::MENU *)>>
            _pause_menu_context;
        bool _is_exit_requested = false;
        bool _return_to_main_menu = false;
        bool _resume_game = false;
        
    };
} // namespace game
