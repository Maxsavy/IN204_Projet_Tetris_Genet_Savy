#include <SFML/Graphics.hpp>
#include "ui/menu.hpp"
#include <iostream>
#include <vector>

using namespace std;

unsigned int windowWidth = 1280;
unsigned int windowHeight = 720;

int main()
{
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Tetris");
    game::MainMenu menu(window);
    menu.start();
    return 0;
}