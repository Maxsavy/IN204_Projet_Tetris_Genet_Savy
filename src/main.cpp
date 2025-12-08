#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <filesystem>

int main()
{
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u{800u, 600u}), "Caca");
    window.setFramerateLimit(144);

    sf::Font font;

    // Try project-relative asset paths first (no leading slash)
    const std::vector<std::filesystem::path> tryPaths = {
        "assets/fonts/arial.ttf",
        "assets/fonts/Arial.ttf"};
    for (const auto &p : tryPaths)
    {
        std::cout << "Trying font: " << p << std::endl;
        if (std::filesystem::exists(p))
            std::cout << "-> path exists\n";
        if (font.openFromFile(p))
        {
            std::cout << "Loaded font: " << p << std::endl;
            break;
        }
    }
    // SFML 3: Text requires a Font reference in its constructor
    sf::Text text{font, "YETI ET MAXIME 2A QUEBEC", 24};
    text.setFillColor(sf::Color::Green);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::Black);
        window.draw(text);
        window.display();
    }

    return 0;
}
