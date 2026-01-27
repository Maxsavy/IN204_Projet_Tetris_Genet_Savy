#include <iostream>
#include "player.hpp"

void Player::generateTetro(Tetro &tempTetro)
{
    if (tetroList.empty())
    {
        tetroList = std::vector<TetroType>(TETRO_BAG.begin(), TETRO_BAG.end());
    }
    int randomIndex = rand() % tetroList.size();
    TetroType selectedType = tetroList[randomIndex];
    tetroList.erase(tetroList.begin() + randomIndex);
    switch (selectedType)
    {
    case TetroType::I:
        tempTetro = TetroI();
        break;
    case TetroType::J:
        tempTetro = TetroJ();
        break;
    case TetroType::L:
        tempTetro = TetroL();
        break;
    case TetroType::O:
        tempTetro = TetroO();
        break;
    case TetroType::S:
        tempTetro = TetroS();
        break;
    case TetroType::T:
        tempTetro = TetroT();
        break;
    case TetroType::Z:
        tempTetro = TetroZ();
        break;
    default:
        tempTetro = TetroI();
        break;
    }

    tempTetro.setPosition(COLUMNS / 2 - 2, -1);
}

void Player::lockTetroInGrid(const Tetro &tetro)
{
    playerGrid.update_with_tetro(tetro, tetro.colorRef);
    generateTetro(currentTetro);
}

void Player::drawDetails(sf::RenderWindow &window, sf::Font &font)
{
    float pixelSize = static_cast<float>(CELL_SIZE * RESIZE_FACTOR);
    float mainGridW = static_cast<float>(COLUMNS) * pixelSize;
    float mainGridH = static_cast<float>(ROWS - 2) * pixelSize;
    float mainOriginX = (static_cast<float>(window.getSize().x) - mainGridW) / 2.f;
    float mainOriginY = (static_cast<float>(window.getSize().y) - mainGridH) / 2.f;
    float offsetX = mainOriginX + mainGridW + 30.f;
    float offsetY = mainOriginY + 200.f;

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Score  " + std::to_string(score));
    scoreText.setCharacterSize(50);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(offsetX, offsetY);
    window.draw(scoreText);

    sf::Text levelText;
    levelText.setFont(font);
    levelText.setString("Level  " + std::to_string(level));
    levelText.setCharacterSize(40);
    levelText.setFillColor(sf::Color::White);
    levelText.setPosition(offsetX, offsetY + 70.f);
    window.draw(levelText);

    sf::Text linesText;
    linesText.setFont(font);
    linesText.setString("Lines Cleared " + std::to_string(playerLinesCleared));
    linesText.setCharacterSize(35);
    linesText.setFillColor(sf::Color::White);
    linesText.setPosition(offsetX, offsetY + 140.f);
    window.draw(linesText);
}

void Player::updateScore(int linesCleared)
{
    switch (linesCleared)
    {
    case 1:
        score += 40;
        break;
    case 2:
        score += 100;
        break;
    case 3:
        score += 300;
        break;
    case 4:
        score += 1200;
        break;
    default:
        break;
    }
}