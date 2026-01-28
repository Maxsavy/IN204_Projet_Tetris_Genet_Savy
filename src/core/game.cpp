#include <iostream>
#include "game.hpp"
#include "../ui/menu.hpp"

#define LOCK_TIME 0.5f

namespace game
{
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
        float targetW = static_cast<float>(COLUMNS) * pixelSize;
        float originX = (static_cast<float>(_window.getSize().x) - targetW) / 2.f;
        float originY = (static_cast<float>(_window.getSize().y) - static_cast<float>(ROWS) * pixelSize) / 2.f;

        player.generateTetro(player.currentTetro);
        player.generateTetro(player.nextTetro);
        gameLoop();
    }

    void GameController::gameLoop()
    {
        bool loopInvarient = true;
        sf::Time paceTime = sf::seconds(0.5f);
        int move_count = 15;
        bool isTouchingGround = false;
        float scaleX = static_cast<float>(_window.getSize().x);
        float scaleY = static_cast<float>(_window.getSize().y);
        while (loopInvarient && _window.isOpen())
        {
            setupScene();
            int colision;
            sf::Event event;
            while (_window.pollEvent(event))
            {
                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Escape)
                    {
                        pause();
                    }

                    if (event.key.code == sf::Keyboard::S)
                    {
                        colision = player.playerGrid.check_collision(player.currentTetro, player.currentTetro.getShape(0), player.currentTetro.position.x, player.currentTetro.position.y + 1);
                        if (colision == 0)
                        {
                            player.currentTetro.moveDown(1);
                            if (isTouchingGround)
                            {
                                player.playerGrid.cancel_locking_timer();
                                move_count--;
                            }
                        }
                    }

                    if (event.key.code == sf::Keyboard::Q)
                    {
                        colision = player.playerGrid.check_collision(player.currentTetro, player.currentTetro.getShape(0), player.currentTetro.position.x - 1, player.currentTetro.position.y);
                        if (colision != 1)
                        {
                            player.currentTetro.moveLeft();
                            if (isTouchingGround)
                            {
                                player.playerGrid.cancel_locking_timer();
                                move_count--;
                            }
                        }
                    }

                    if (event.key.code == sf::Keyboard::D)
                    {
                        colision = player.playerGrid.check_collision(player.currentTetro, player.currentTetro.getShape(0), player.currentTetro.position.x + 1, player.currentTetro.position.y);
                        if (colision != 1)
                        {
                            player.currentTetro.moveRight();
                            if (isTouchingGround)
                            {
                                player.playerGrid.cancel_locking_timer();
                                move_count--;
                            }
                        }
                    }

                    if (event.key.code == sf::Keyboard::Z)
                    {
                        auto newShape = player.currentTetro.getShape(1);
                        int currentRotation = player.currentTetro.currentRotation;
                        auto kick = player.playerGrid.try_wall_kicks(player.currentTetro, newShape, currentRotation);
                        colision = player.playerGrid.check_collision(
                            player.currentTetro, newShape, player.currentTetro.position.x + kick.first, player.currentTetro.position.y + kick.second);

                        if (colision == 0 || colision == 1)
                        {
                            player.currentTetro.rotate();
                            player.currentTetro.position.x += kick.first;
                            player.currentTetro.position.y += kick.second;
                            if (isTouchingGround)
                            {
                                player.playerGrid.cancel_locking_timer();
                                move_count--;
                            }
                        }
                    }
                }
                if (event.type == sf::Event::Closed)
                {
                    loopInvarient = false;
                    break;
                }
            } // event loop

            // Vérifier si la pièce touche le sol
            colision = player.playerGrid.check_collision(player.currentTetro, player.currentTetro.getShape(0), player.currentTetro.position.x, player.currentTetro.position.y + 1);
            isTouchingGround = (colision == 2);

            if (gameClock.getElapsedTime().asSeconds() >= paceTime.asSeconds())
            {
                gameClock.restart();
                if (colision == 0)
                {
                    player.currentTetro.moveDown(1);
                }
                else if (colision == 2)
                {
                    if (player.playerGrid.locking_tetro(player.currentTetro, move_count))
                    {
                        player.currentTetro = player.nextTetro;
                        player.generateTetro(player.nextTetro);
                        player.currentTetro.moveDown(2);
                        move_count = 15;
                        isTouchingGround = false;
                    }
                }
                else if (colision == 3)
                {
                    gameOver();
                }
            }
            
            if (gameClock.getElapsedTime().asSeconds() >= 0.3f)
            {
                std::vector<int> deletedRows = player.playerGrid.delete_full_rows();
                if (!deletedRows.empty())
                    player.playerGrid.draw_deleted_row_animation(_window, deletedRows);
                player.updateScore(player.playerGrid.linesCleared);
                player.playerGrid.linesCleared = 0;
            }

            _window.display();
            _window.setFramerateLimit(60);
            player.level = player.playerGrid.totalLinesCleared / 10 + 1;
            paceTime = sf::seconds(0.5f - (static_cast<float>(player.playerGrid.totalLinesCleared / 10) * 0.08f));
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

        player.playerGrid.drawGameGrid(_window, player.currentTetro, player.playerGrid.rows, player.playerGrid.columns);
        player.playerGrid.drawNextGrid(_window, player.nextTetro);
        player.drawDetails(_window, font);
        player.playerGrid.drawGhostTetro(_window, player.currentTetro);
    }

    void GameController::gameOver()
    {
        game::MainMenu menu(_window);
        menu.showGameOverMenu();
    }

    void GameController::pause()
    {
        game::MainMenu menu(_window);
        menu.showPauseMenu(this);
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
        loadTexture(background, "assets/images/background_game.jpg", background_sprite);

        loadTexture(grid, "assets/images/tetris_grid.png", grid_sprite);

        loadTexture(logo, "assets/images/tetris_logo.png", logo_sprite);

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
