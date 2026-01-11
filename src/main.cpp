#include <SFML/Graphics.hpp>
#include "ui/menu.cpp"
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Tetris");
    game::MainMenu menu(window);
    menu.start();
    return 0;
}