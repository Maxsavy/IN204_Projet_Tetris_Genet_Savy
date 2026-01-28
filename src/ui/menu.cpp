#include "menu.hpp"
#include "../core/game.hpp"
#include "../core/leaderboard.hpp"
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

    void MainMenu::showGameOverMenu(int finalScore, int finalLevel)
    {
        _return_to_main_menu = false;
        _window.setFramerateLimit(144);

        std::string playerName = "";
        bool enteringName = true;
        const int maxNameLength = 15;

        sf::Font font;
        if (!font.loadFromFile("assets/fonts/arcade.ttf"))
        {
            std::cerr << "Failed to load font" << std::endl;
            return;
        }

        sf::Text titleText;
        titleText.setFont(font);
        titleText.setString("GAME OVER");
        titleText.setCharacterSize(60);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(_window.getSize().x / 2.0f - 180.0f, 100.0f);

        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setString("Score  " + std::to_string(finalScore));
        scoreText.setCharacterSize(40);
        scoreText.setFillColor(sf::Color::Cyan);
        scoreText.setPosition(_window.getSize().x / 2.0f - 150.0f, 200.0f);

        sf::Text levelText;
        levelText.setFont(font);
        levelText.setString("Level  " + std::to_string(finalLevel));
        levelText.setCharacterSize(40);
        levelText.setFillColor(sf::Color::Cyan);
        levelText.setPosition(_window.getSize().x / 2.0f - 150.0f, 260.0f);

        sf::Text promptText;
        promptText.setFont(font);
        promptText.setString("Enter your name");
        promptText.setCharacterSize(35);
        promptText.setFillColor(sf::Color::White);
        promptText.setPosition(_window.getSize().x / 2.0f - 180.0f, 350.0f);

        sf::Text nameText;
        nameText.setFont(font);
        nameText.setString("_");
        nameText.setCharacterSize(45);
        nameText.setFillColor(sf::Color::Yellow);
        nameText.setPosition(_window.getSize().x / 2.0f - 100.0f, 420.0f);

        sf::Text instructionText;
        instructionText.setFont(font);
        instructionText.setString("Press ENTER to continue");
        instructionText.setCharacterSize(25);
        instructionText.setFillColor(sf::Color(200, 200, 200));
        instructionText.setPosition(_window.getSize().x / 2.0f - 200.0f, 520.0f);

        sf::Clock blinkClock;
        bool showCursor = true;

        while (_window.isOpen() && enteringName)
        {
            sf::Event event;
            while (_window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    _window.close();
                    return;
                }

                if (event.type == sf::Event::TextEntered)
                {
                    if (event.text.unicode == '\b' && playerName.length() > 0)
                    {
                        playerName.pop_back();
                    }
                    else if (event.text.unicode == '\r' || event.text.unicode == '\n')
                    {
                        if (playerName.empty())
                        {
                            playerName = "Player";
                        }
                        enteringName = false;
                    }
                    else if (event.text.unicode < 128 && event.text.unicode >= 32 &&
                             playerName.length() < maxNameLength)
                    {
                        playerName += static_cast<char>(event.text.unicode);
                    }
                }
            }

            if (blinkClock.getElapsedTime().asSeconds() >= 0.5f)
            {
                showCursor = !showCursor;
                blinkClock.restart();
            }

            std::string displayName = playerName;
            if (showCursor && enteringName)
            {
                displayName += "_";
            }
            nameText.setString(displayName);

            sf::FloatRect textBounds = nameText.getLocalBounds();
            nameText.setPosition(_window.getSize().x / 2.0f - textBounds.width / 2.0f, 420.0f);

            _window.clear();
            if (background.getSize().x > 0 && background.getSize().y > 0)
            {
                _window.draw(background_sprite);
            }
            _window.draw(titleText);
            _window.draw(scoreText);
            _window.draw(levelText);
            _window.draw(promptText);
            _window.draw(nameText);
            _window.draw(instructionText);
            _window.display();
        }

        // Save to leaderboard
        if (!playerName.empty())
        {
            Leaderboard leaderboard;
            leaderboard.addEntry("src/core/leaderboard.txt", playerName, finalScore, finalLevel);
        }

        // Return to main menu
        _return_to_main_menu = true;
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
            {"New  Solo  Game",
             [&](sf::RenderTarget &target)
             {
                 game::GameController gameController(_window);
                 last_selected_mode = 1;
                 gameController.start(1);
             }},
            {"New  Split Screen Game",
             [&](sf::RenderTarget &target)
             {
                 game::GameController gameController(_window);
                 last_selected_mode = 2;
                 gameController.start(2);
             }},
            {"Leaderboard", [](sf::RenderTarget &target)
             {
                 Leaderboard leaderboard;
                 leaderboard.start(static_cast<sf::RenderWindow &>(target));
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
                 gameController.start(last_selected_mode);
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
                 gameController.start(last_selected_mode);
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