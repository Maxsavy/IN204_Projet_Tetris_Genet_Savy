#include <SFML/Graphics.hpp>
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
        MainMenu(sf::RenderWindow &w);
        void start();
        void showGameOverMenu();
        void showPauseMenu(GameController *gameController);

    private:
        void setup_menu_context();
        void setup_game_over_menu();

        sf::RenderWindow &_window;
        sf::Font _font;
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
        int last_selected_mode;
    };
} // namespace game
