#include <iostream>
#include "game.hpp"
#include "../ui/menu.hpp"

#define LOCK_TIME 0.5f

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
        int score = 0;
        int move_count = 15;

        float scaleX = static_cast<float>(_window.getSize().x);
        float scaleY = static_cast<float>(_window.getSize().y);
        while (loopInvarient)
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
                        game::MainMenu menu(_window);
                        menu.start();
                        exit(0);
                    }

                    if (event.key.code == sf::Keyboard::S)
                    {
                        colision = player.playerGrid.check_collision(player.currentTetro, player.currentTetro.getShape(0), player.currentTetro.position.x, player.currentTetro.position.y + 1);
                        if (colision == 0)
                            player.currentTetro.moveDown(1);
                    }

                    if (event.key.code == sf::Keyboard::Q)
                    {
                        colision = player.playerGrid.check_collision(player.currentTetro, player.currentTetro.getShape(0), player.currentTetro.position.x - 1, player.currentTetro.position.y);
                        if (colision == 0 || colision == 2)
                            player.currentTetro.moveLeft();
                    }

                    if (event.key.code == sf::Keyboard::D)
                    {
                        colision = player.playerGrid.check_collision(player.currentTetro, player.currentTetro.getShape(0), player.currentTetro.position.x + 1, player.currentTetro.position.y);
                        if (colision == 0 || colision == 2)
                            player.currentTetro.moveRight();
                    }

                    if (event.key.code == sf::Keyboard::Z)
                    {
                        // Obtenir la forme après rotation
                        auto newShape = player.currentTetro.getShape(1);
                        int currentRotation = player.currentTetro.currentRotation; // Ajoutez ce membre si nécessaire

                        // Tester la rotation avec wall kicks
                        auto kick = player.playerGrid.try_wall_kicks(player.currentTetro, newShape, currentRotation);

                        // Vérifier si un kick a réussi
                        colision = player.playerGrid.check_collision(
                            player.currentTetro, newShape, player.currentTetro.position.x + kick.first, player.currentTetro.position.y + kick.second);

                        if (colision == 0 || colision == 1) // Pas de collision critique
                        {
                            player.currentTetro.rotate();
                            player.currentTetro.position.x += kick.first;
                            player.currentTetro.position.y += kick.second;
        
                        }
                    }
                }
                if (event.type == sf::Event::Closed)
                {
                    game::MainMenu menu(_window);
                    menu.start();
                    exit(0);
                }

                // Check for player movement to reset locking timer
                if (player.hasMadeMove(event) != 0)
                    {
                        player.playerGrid.cancel_locking_timer();
                        if (colision == 2)
                        {
                            move_count--;
                            std::cout << "Move count decreased to: " << move_count << std::endl;
                        }
                    }
            } // event loop

            
            if (gameClock.getElapsedTime().asSeconds() >= paceTime.asSeconds()) 
            {
                gameClock.restart();
                colision = player.playerGrid.check_collision(player.currentTetro, player.currentTetro.getShape(0), player.currentTetro.position.x, player.currentTetro.position.y + 1);
                if (colision == 0){
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
                    }

                }
                else if (colision == 3)
                {
                    gameOver();
                }   
            }
            if (gameClock.getElapsedTime().asSeconds() >= 0.3f)
            {
                player.playerGrid.delete_full_rows(score);
                
            }
            player.hasMadeMove(event);
            // paceTime = sf::seconds(0.5f - (static_cast<float>(score) * 0.08f));
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

        player.playerGrid.drawGrid(_window, player.currentTetro);
    }


    void GameController::gameOver()
    {
        game::MainMenu menu(_window);
        menu.start();
        exit(0);
    }

    // Mis en commentaire car je pense que c'est inutile

    // bool checkCollision(const sf::RectangleShape &a, const sf::RectangleShape &b)
    // {
    //     return a.getGlobalBounds().intersects(b.getGlobalBounds());
    // }

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
