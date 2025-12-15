#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    // --- LOADING FONT AND IMAGE ---
    sf::Font font;

    if (!font.loadFromFile("assets/fonts/arcade.ttf"))
    {
        cout << "pas de font lol" << endl;
        return -1;
    }

    sf::Texture background;
    if (!background.loadFromFile("assets/images/background_menu.jpg"))
    {
        cout << "pas d'image lol" << endl;
        return -1;
    }

    // --- SETTING THE WINDOW ---
    unsigned int windowWidth = 1280;
    unsigned int windowHeight = 720;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Tetris");
    window.setFramerateLimit(144);

    // --- SETUP SPRITE ---
    sf::Sprite back_sprite(background);

    sf::Vector2u imageSize = background.getSize();
    float scaleX = static_cast<float>(windowWidth) / imageSize.x;
    float scaleY = static_cast<float>(windowHeight) / imageSize.y;

    back_sprite.setScale(scaleX, scaleY);

    // --- SETTING TITLE ---
    sf::Text title("TETRIS SA MERE", font, 90);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold);
    title.setPosition(550.f, 100.f);

    // --- GAME LOOP ---
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        window.draw(back_sprite);
        window.draw(title);
        window.display();
    }

    return 0;
}