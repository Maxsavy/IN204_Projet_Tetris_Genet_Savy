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

    void GameController::start(int mode)
    {
        this->modePLayed = mode;
        loadResources();
        float windowW = static_cast<float>(_window.getSize().x);
        float windowH = static_cast<float>(_window.getSize().y);

        if (mode == 1)
        {
            player[0].initializeSinglePlayer();
            player[0].playerGrid.setGridPosition(1, 0, windowW, windowH);
        }
        else if (mode == 2)
        {
            player[0].initializeSplitScreenPlayer(0);
            player[0].playerGrid.setGridPosition(2, 0, windowW, windowH);
            player[1].initializeSplitScreenPlayer(1);
            player[1].playerGrid.setGridPosition(2, 1, windowW, windowH);
        }

        gameLoop();
    }

    void GameController::gameLoop()
    {
        sf::Time paceTime = sf::seconds(0.5f);

        int moveCount[2] = {15, 15};
        bool isTouchingGround[2] = {false, false};

        int numPlayers = (modePLayed == 2) ? 2 : 1;

        while (_window.isOpen())
        {
            setupScene();

            sf::Event event;
            while (_window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    _window.close();
                    return;
                }

                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Escape)
                    {
                        pause();
                    }

                    handlePlayerInput(0, event, moveCount[0], isTouchingGround[0]);

                    if (modePLayed == 2)
                    {
                        handlePlayerInput(1, event, moveCount[1], isTouchingGround[1]);
                    }
                }
            } // event loop

            for (int p = 0; p < numPlayers; p++)
            {

                int collision = player[p].playerGrid.check_collision(
                    player[p].currentTetro,
                    player[p].currentTetro.getShape(0),
                    player[p].currentTetro.position.x,
                    player[p].currentTetro.position.y + 1);
                isTouchingGround[p] = (collision == 2);

                if (gameClock.getElapsedTime().asSeconds() >= paceTime.asSeconds())
                {
                    if (collision == 0)
                    {
                        player[p].currentTetro.moveDown(1);
                    }
                    else if (collision == 2)
                    {

                        if (player[p].playerGrid.locking_tetro(player[p].currentTetro, moveCount[p]))
                        {
                            player[p].currentTetro = player[p].nextTetro;
                            player[p].generateTetro(player[p].nextTetro);
                            player[p].currentTetro.moveDown(2);
                            moveCount[p] = 15;
                            isTouchingGround[p] = false;
                        }
                    }
                    else if (collision == 3)
                    {
                        gameOver();
                        return;
                    }
                }

                if (gameClock.getElapsedTime().asSeconds() >= 0.3f)
                {
                    std::vector<int> deletedRows = player[p].playerGrid.delete_full_rows();
                    if (!deletedRows.empty())
                    {
                        player[p].playerGrid.draw_deleted_row_animation(_window, deletedRows);
                    }
                    player[p].updateScore(player[p].playerGrid.linesCleared);
                    player[p].playerGrid.linesCleared = 0;
                }

                player[p].level = player[p].playerGrid.totalLinesCleared / 10 + 1;
            }

            if (gameClock.getElapsedTime().asSeconds() >= paceTime.asSeconds())
            {
                gameClock.restart();
            }

            paceTime = sf::seconds(0.5f - (static_cast<float>(player[0].playerGrid.totalLinesCleared / 10) * 0.08f));

            _window.display();
            _window.setFramerateLimit(60);
        }
    }

    void GameController::handlePlayerInput(int playerId, sf::Event &event, int &moveCount, bool &isTouchingGround)
    {
        Player &p = player[playerId];
        int collision;

        // Define controls based on player
        sf::Keyboard::Key downKey = (playerId == 0) ? sf::Keyboard::S : sf::Keyboard::Down;
        sf::Keyboard::Key leftKey = (playerId == 0) ? sf::Keyboard::Q : sf::Keyboard::Left;
        sf::Keyboard::Key rightKey = (playerId == 0) ? sf::Keyboard::D : sf::Keyboard::Right;
        sf::Keyboard::Key rotateKey = (playerId == 0) ? sf::Keyboard::Z : sf::Keyboard::Up;

        // Move down
        if (event.key.code == downKey)
        {
            collision = p.playerGrid.check_collision(
                p.currentTetro, p.currentTetro.getShape(0),
                p.currentTetro.position.x, p.currentTetro.position.y + 1);
            if (collision == 0)
            {
                p.currentTetro.moveDown(1);
                if (isTouchingGround)
                {
                    p.playerGrid.cancel_locking_timer();
                    moveCount--;
                }
            }
        }

        // Move left
        if (event.key.code == leftKey)
        {
            collision = p.playerGrid.check_collision(
                p.currentTetro, p.currentTetro.getShape(0),
                p.currentTetro.position.x - 1, p.currentTetro.position.y);
            if (collision != 1)
            {
                p.currentTetro.moveLeft();
                if (isTouchingGround)
                {
                    p.playerGrid.cancel_locking_timer();
                    moveCount--;
                }
            }
        }

        // Move right
        if (event.key.code == rightKey)
        {
            collision = p.playerGrid.check_collision(
                p.currentTetro, p.currentTetro.getShape(0),
                p.currentTetro.position.x + 1, p.currentTetro.position.y);
            if (collision != 1)
            {
                p.currentTetro.moveRight();
                if (isTouchingGround)
                {
                    p.playerGrid.cancel_locking_timer();
                    moveCount--;
                }
            }
        }

        // Rotate
        if (event.key.code == rotateKey)
        {
            auto newShape = p.currentTetro.getShape(1);
            int currentRotation = p.currentTetro.currentRotation;
            auto kick = p.playerGrid.try_wall_kicks(p.currentTetro, newShape, currentRotation);
            collision = p.playerGrid.check_collision(
                p.currentTetro, newShape,
                p.currentTetro.position.x + kick.first,
                p.currentTetro.position.y + kick.second);

            if (collision == 0 || collision == 1)
            {
                p.currentTetro.rotate();
                p.currentTetro.position.x += kick.first;
                p.currentTetro.position.y += kick.second;
                if (isTouchingGround)
                {
                    p.playerGrid.cancel_locking_timer();
                    moveCount--;
                }
            }
        }
    }

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

        player[0].playerGrid.drawGameGrid(_window, player[0].currentTetro);
        player[0].playerGrid.drawNextGrid(_window, player[0].nextTetro);
        player[0].drawDetails(_window, font, modePLayed, 0);
        player[0].playerGrid.drawGhostTetro(_window, player[0].currentTetro);

        if (modePLayed == 2)
        {
            player[1].playerGrid.drawGameGrid(_window, player[1].currentTetro);
            player[1].playerGrid.drawNextGrid(_window, player[1].nextTetro);
            player[1].drawDetails(_window, font, modePLayed, 1);
            player[1].playerGrid.drawGhostTetro(_window, player[1].currentTetro);
        }
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
