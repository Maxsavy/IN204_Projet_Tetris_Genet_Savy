#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <filesystem>

constexpr  int ROWS = 20;
constexpr int COLUMNS = 10;
constexpr  int CELL_SIZE = 8;
constexpr  int RESIZE_FACTOR = 4;

// int main()
// {
//     sf::RenderWindow window(sf::VideoMode(COLUMNS * CELL_SIZE * RESIZE_FACTOR, ROWS * CELL_SIZE * RESIZE_FACTOR), "La fenetre SFML");
//     window.setFramerateLimit(144);

//     sf::RectangleShape rectangle(sf::Vector2f((CELL_SIZE-1) * RESIZE_FACTOR, (CELL_SIZE-1) * RESIZE_FACTOR));
//     rectangle.setFillColor(sf::Color::Green);


//     while (window.isOpen())
//     {
//         sf::Event event;
//         while (window.pollEvent(event))
//         {
//             if (event.type == sf::Event::Closed)
//                 window.close();

//             if (event.type == sf::Event::KeyPressed)
//             {
//                 if (event.key.code == sf::Keyboard::Escape)
//                     window.close();

//                 if (event.key.code == sf::Keyboard::Down)
//                 {
//                     rectangle.move(0, CELL_SIZE * RESIZE_FACTOR);
//                 }

//                 if (event.key.code == sf::Keyboard::Left)
//                 {
//                     rectangle.move(-CELL_SIZE * RESIZE_FACTOR, 0);
//                 }

//                 if (event.key.code == sf::Keyboard::Right)
//                 {
//                     rectangle.move(CELL_SIZE * RESIZE_FACTOR, 0);
//                 }

//                 if (event.key.code == sf::Keyboard::Up)
//                 {
//                     rectangle.move(0, -CELL_SIZE * RESIZE_FACTOR);
//                     std::cout << "Position: (" << rectangle.getPosition().x << ", " << rectangle.getPosition().y << ")" << std::endl;
//                 }
//             }

//         }

//         window.clear(sf::Color::Black);
//         window.draw(rectangle);
//         window.display();
//     }

//     return 0;
// }
