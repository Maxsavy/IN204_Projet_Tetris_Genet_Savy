#include "menu.hpp"
#include "../core/game.hpp"
#include <iostream>
using namespace std;

namespace game
{
    sf::Texture background;
    sf::Sprite background_sprite;

    MainMenu::MainMenu(sf::RenderWindow &w)
        : _window(w), _main_menu_context(nullptr, menu_destroy_context), _game_over_menu_context(nullptr, menu_destroy_context), _pause_menu_context(nullptr, menu_destroy_context)
    {
        setup_menu_context();
    }

    void MainMenu::start()
    {
        _window.setFramerateLimit(144);
        _current_menu = _main_menu_context.get();
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
                if (event.type == sf::Event::Closed ||
                    (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
                {
                    _is_exit_requested = true;
                    break;
                }

                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Z)
                    {
                        event.key.code = sf::Keyboard::Up;
                    }
                    else if (event.key.code == sf::Keyboard::S)
                    {
                        event.key.code = sf::Keyboard::Down;
                    }
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

    void MainMenu::showGameOverMenu()
    {
        _return_to_main_menu = false;
        _current_menu = _game_over_menu_context.get();
        _window.setFramerateLimit(144);
        while (_window.isOpen() && !_return_to_main_menu)
        {
            sf::Event event;
            while (_window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed ||
                    (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
                {
                    _return_to_main_menu = true;
                    break;
                }

                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Z)
                    {
                        event.key.code = sf::Keyboard::Up;
                    }
                    else if (event.key.code == sf::Keyboard::S)
                    {
                        event.key.code = sf::Keyboard::Down;
                    }
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
        _current_menu = _main_menu_context.get();
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

        if (!_background_music.openFromFile("assets/sounds/menu_theme.mp3"))
        {
            cerr << "Failed to load music file" << endl;
        }
        else
        {
            _background_music.setLoop(true);  // Active la boucle automatique
            _background_music.setVolume(50);   // Volume à 50% (optionnel)
            _background_music.play();
        }

        game_menu::Style style{.TitleFont = &_font,
                               .ItemFont = &_font,
                               .TitleFontSize = 60,
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
                 _background_music.stop();
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
             { this->_is_exit_requested = true; 
               _window.close(); }},
        };

        game_menu::MenuConfig config{
            .title = "Retro  Tetris", .items = items, .style = style};
        _main_menu_context.reset(create_menu_context(_window, config));
        _current_menu = _main_menu_context.get();

        setup_game_over_menu();
    }

    void MainMenu::setup_game_over_menu()
    {
        game_menu::Style style{.TitleFont = &_font,
                               .ItemFont = &_font,
                               .TitleFontSize = 60,
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
            {"Replay",
             [&](sf::RenderTarget &target)
             {
                 game::GameController gameController(_window);
                 _background_music.stop();
                 gameController.start();
             }},
            {"Return  to  Menu", [&](sf::RenderTarget &target)
             {
                 _return_to_main_menu = true;
                 _current_menu = _main_menu_context.get();
                 start();
             }}};

        game_menu::MenuConfig config{
            .title = " Game Over", .items = items, .style = style};
        _game_over_menu_context.reset(create_menu_context(_window, config));
    }

    void MainMenu::showPauseMenu(GameController *gameController)
    {
        _return_to_main_menu = false;
        _resume_game = false;

        game_menu::Style style{.TitleFont = &_font,
                               .ItemFont = &_font,
                               .TitleFontSize = 60,
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
            {"Resume",
             [&](sf::RenderTarget &target)
             {
                 _resume_game = true;
             }},
            {"Replay",
             [&](sf::RenderTarget &target)
             {
                 _resume_game = false;
                 GameController gameController(_window);
                 _background_music.stop();
                 gameController.start();
             }},
            {"Return  to  Menu", [&](sf::RenderTarget &target)
             {
                 _return_to_main_menu = true;
                 _current_menu = _main_menu_context.get();
             }}};

        game_menu::MenuConfig config{
            .title = " PAUSE", .items = items, .style = style};

        _pause_menu_context.reset(create_menu_context(_window, config));
        _current_menu = _pause_menu_context.get();

        _window.setFramerateLimit(144);
        while (_window.isOpen() && !_return_to_main_menu && !_resume_game)
        {
            sf::Event event;
            while (_window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    _window.close();
                    return;
                }
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                {
                    _resume_game = true;
                    break;
                }

                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Z)
                    {
                        event.key.code = sf::Keyboard::Up;
                    }
                    else if (event.key.code == sf::Keyboard::S)
                    {
                        event.key.code = sf::Keyboard::Down;
                    }
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

        if (_return_to_main_menu)
        {
            _current_menu = _main_menu_context.get();
            start();
        }
    }
}