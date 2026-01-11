#include "menu.hpp"
#include "../core/game.hpp"
#include <iostream>
using namespace std;



namespace game
{
    sf::Texture background;
    sf::Sprite background_sprite;

    MainMenu::MainMenu(sf::RenderWindow &w)
        : _window(w), _main_menu_context(nullptr, menu_destroy_context)
    {
        setup_menu_context();
    }

    void MainMenu::start()
    {
        _window.setFramerateLimit(144);
        while (_window.isOpen())
        {
            if (_is_exit_requested)
            {
                _window.close();
                break;
            }
            sf::Event event;
            while (_window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    _is_exit_requested = true;
                    break;
                }
                menu_handle_event(_current_menu, event);
            }
            _window.clear();
            if (background.getSize().x > 0 && background.getSize().y > 0)
            {
                _window.draw(background_sprite);
            }
            menu_render(_current_menu);
            _window.display();
        }
    }

    void MainMenu::setup_menu_context()
    {
        if (!_font.loadFromFile("assets/fonts/arcade.ttf"))
        {
            cerr << "Failed to load font: arcade.ttf (tried multiple locations)" << endl;
        }

        if (!background.loadFromFile("assets/images/background_menu.jpg"))
        {
            cerr << "Failed to load background image: assets/images/background_menu.jpg" << endl;
        }
        else
        {
            background_sprite.setTexture(background);
            sf::Vector2u imageSize = background.getSize();
            if (imageSize.x > 0 && imageSize.y > 0)
            {
                float scaleX = static_cast<float>(_window.getSize().x) / imageSize.x;
                float scaleY = static_cast<float>(_window.getSize().y) / imageSize.y;
                background_sprite.setScale(scaleX, scaleY);
                background_sprite.setPosition(0.f, 0.f);
            }
        }
        game_menu::Style style{.TitleFont = &_font,
                               .ItemFont = &_font,
                               .TitleFontSize = 50,
                               .ItemFontSize = 35,
                               .MenuTitleScaleFactor = 1,
                               .MenuItemScaleFactor = 1.5,
                               .colorScheme = {.titleColor = 0xFFFFFF,
                                               .itemColor = 0xFFFFFF,
                                               .selectedColor = 0xFF22F1},
                               .PaddingTitle =
                                   {
                                       .top = 100,
                                       .left = 0,
                                   },
                               .PaddingItems =
                                   {
                                       .top = 30,
                                   },
                               .TitleAlign = game_menu::Align::Center,
                               .ItemAlign = game_menu::Align::Center};

        std::vector<game_menu::MenuItem> items{
            {"New Game",
             [&](sf::RenderTarget &target)
             {
                 game::GameController gameController(_window);
                 gameController.start();
             }},
            {"Leaderboard", [](sf::RenderTarget &target)
             {
                 cout << "leaderboard!" << endl;
             }},
            {"Settings", [](sf::RenderTarget &target)
             {
                 cout << "settings!" << endl;
             }},
            {"Exit", [&](sf::RenderTarget &target)
             { this->_is_exit_requested = true; }}};

        game_menu::MenuConfig config{
            .title = " Tetris ouiii", .items = items, .style = style};
        _main_menu_context.reset(create_menu_context(_window, config));
        _current_menu = _main_menu_context.get();
    }
}