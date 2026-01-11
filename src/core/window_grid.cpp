#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <filesystem>

#include "grid.hpp"

constexpr  int ROWS = 20;
constexpr int COLUMNS = 10;
constexpr  int CELL_SIZE = 8;
constexpr  int RESIZE_FACTOR = 4;

int main()
{
    sf::RenderWindow window(sf::VideoMode(COLUMNS * CELL_SIZE * RESIZE_FACTOR, ROWS * CELL_SIZE * RESIZE_FACTOR), "La fenetre SFML");
    window.setFramerateLimit(144);

    sf::Clock clock;
    unsigned int seconds = 0;

    sf::RectangleShape cell(sf::Vector2f((CELL_SIZE-1) * RESIZE_FACTOR, (CELL_SIZE-1) * RESIZE_FACTOR));
    sf::Color empty(0,0,100);
    sf::Color filled(255,0,0);


    Grid grid;

    grid.display_terminal();

    Tetro tetro;


    while (window.isOpen())
    {
       
        // Gestion des événements

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();

            }
        }

        if (clock.getElapsedTime().asSeconds() >= 0.1f) {
            ++seconds;
            clock.restart();

            grid.update_with_tetro(tetro);
            grid.display_terminal();
            tetro.move_down();

        }
        

        // Redessinage de la grille à chaque frame
        window.clear(sf::Color::Black);
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLUMNS; j++) {
                cell.setPosition(j * CELL_SIZE * RESIZE_FACTOR, i * CELL_SIZE * RESIZE_FACTOR);
                if (grid.cells[i * COLUMNS + j] == 0) {
                    cell.setFillColor(empty);
                } else {
                    cell.setFillColor(filled);
                }
                window.draw(cell);
            }
        }
        window.display();
    }

    return 0;
}
