#include <iostream>
#include "game.hpp"
#include "../ui/menu.hpp"
namespace game
{
    // constructor
    GameController::GameController(sf::RenderWindow &w)
        : _window(w)
    {
        score = 0;
        direction = {0, 0};
        scale = 1.0f;
    }

    void GameController::start()
    {
        loadResources();
        float pixelSize = static_cast<float>(CELL_SIZE * RESIZE_FACTOR);
        float targetW = static_cast<float>(real_grid.columns) * pixelSize;
        float originX = (static_cast<float>(_window.getSize().x) - targetW) / 2.f;
        float originY = (static_cast<float>(_window.getSize().y) - static_cast<float>(real_grid.rows) * pixelSize) / 2.f;

        real_grid.generateTetro();
        gameLoop();
    }

    void GameController::gameLoop()
    {
        bool loopInvarient = true;
        float scaleX = static_cast<float>(_window.getSize().x);
        float scaleY = static_cast<float>(_window.getSize().y);
        while (loopInvarient)
        {
            setupScene();
            sf::Event event;
            while (_window.pollEvent(event))
            {
                if (event.type == sf::Event::KeyReleased)
                {
                    if (event.key.code == sf::Keyboard::Escape)
                    {
                        game::MainMenu menu(_window);
                        menu.start();
                        exit(0);
                    }

                    if (event.key.code == sf::Keyboard::Down)
                    {
                        real_grid.tempTetro.position.y++;
                    }

                    if (event.key.code == sf::Keyboard::Left)
                    {
                        real_grid.tempTetro.position.x--;
                    }

                    if (event.key.code == sf::Keyboard::Right)
                    {
                        real_grid.tempTetro.position.x++;
                    }

                    if (event.key.code == sf::Keyboard::Up)
                    {
                        real_grid.tempTetro.rotate();
                    }
                    if (event.key.code == sf::Keyboard::L)
                    {
                        real_grid.lockTetroInGrid(real_grid.tempTetro);
                    }
                }
                if (event.type == sf::Event::Closed)
                {
                    exit(0);
                }
            } // event loop
            _window.display();
            _window.setFramerateLimit(60);
        }
    } // gameLoop()

    void GameController::setupScene()
    {
        _window.clear();

        if (background_sprite.getTexture())
        {
            setUpBackground(background, background_sprite);
            _window.draw(background_sprite);
        }

        if (logo_sprite.getTexture())
        {
            logo_sprite.setScale(0.2F, 0.2F);
            logo_sprite.setPosition(20, 20);
            _window.draw(logo_sprite);
        }

        real_grid.drawGrid(_window);
    }

    bool checkCollision(const sf::RectangleShape &a, const sf::RectangleShape &b)
    {
        return a.getGlobalBounds().intersects(b.getGlobalBounds());
    }

    sf::RectangleShape getRectangleAt(sf::Vector2f location, sf::Color color)
    {
        sf::RectangleShape box;
        box.setSize(sf::Vector2f(BOX_SIZE, BOX_SIZE));
        box.setPosition(location);
        box.setFillColor(color);
        return box;
    }

    void GameController::loadResources()
    {
        // background loading and padding
        loadTexture(background, "assets/images/background_game.jpg", background_sprite);

        // grid loading and padding
        loadTexture(grid, "assets/images/tetris_grid.png", grid_sprite);

        // logo loading and padding
        loadTexture(logo, "assets/images/tetris_logo.png", logo_sprite);

        // font loading
        if (!font.loadFromFile("assets/fonts/arcade.ttf"))
        {
            std::cerr << "Failed to load font: arcade.ttf" << std::endl;
        }
    }

    void GameController::loadTexture(sf::Texture &texture, const std::string &file, sf::Sprite &sprite)
    {
        if (!texture.loadFromFile(file))
        {
            std::cerr << "Failed to load texture " << file << std::endl;
        }
        sprite.setTexture(texture);
    }

    void GameController::setUpBackground(sf::Texture &texture, sf::Sprite &sprite)
    {
        sf::Vector2u imageSize = texture.getSize();
        if (imageSize.x > 0 && imageSize.y > 0)
        {
            float scaleX = static_cast<float>(_window.getSize().x) / imageSize.x;
            float scaleY = static_cast<float>(_window.getSize().y) / imageSize.y;
            sprite.setScale(scaleX, scaleY);
            sprite.setPosition(0.f, 0.f);
        }
    }

} // namespace game
