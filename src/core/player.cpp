#include <iostream>
#include "player.hpp"

// each player has a tetro that he controls, this function generates it randomly from the TETRO_BAG defined in pieces.hpp
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

int Player::hasMadeMove(sf::Event &event)
{
    if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Q || event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Z))
    {
        return 1;
    }
    return 0;
}

// function to draw the details : score, level and lines cleared of the player
void Player::drawDetails(sf::RenderWindow &window, sf::Font &font, int modePlayed, int playerID)
{
    float pixelSize = static_cast<float>(CELL_SIZE * RESIZE_FACTOR);
    float gridWidth = static_cast<float>(COLUMNS) * pixelSize;

    float offsetX = playerGrid.originX + gridWidth + 30.f;
    float offsetY = playerGrid.originY + 200.f;

    int scoreSize = (modePlayed == 2) ? 35 : 50;
    int levelSize = (modePlayed == 2) ? 30 : 40;
    int linesSize = (modePlayed == 2) ? 30 : 35;
    float verticalSpacing = (modePlayed == 2) ? 50.f : 70.f;

    // if 2 players, this shows P1 and P2
    if (modePlayed == 2)
    {
        sf::Text playerLabel;
        playerLabel.setFont(font);
        playerLabel.setString("P" + std::to_string(playerID + 1));
        playerLabel.setCharacterSize(40);
        playerLabel.setFillColor(sf::Color::Yellow);
        playerLabel.setPosition(offsetX, offsetY - 70.f);
        window.draw(playerLabel);
    }

    // all the texts to display the details
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Score");
    scoreText.setCharacterSize(scoreSize);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(offsetX, offsetY);
    window.draw(scoreText);

    sf::Text scoreValue;
    scoreValue.setFont(font);
    scoreValue.setString(std::to_string(score));
    scoreValue.setCharacterSize(scoreSize - 5);
    scoreValue.setFillColor(sf::Color::Cyan);
    scoreValue.setPosition(offsetX, offsetY + 35.f);
    window.draw(scoreValue);

    sf::Text levelText;
    levelText.setFont(font);
    levelText.setString("Level  " + std::to_string(level));
    levelText.setCharacterSize(levelSize);
    levelText.setFillColor(sf::Color::White);
    levelText.setPosition(offsetX, offsetY + verticalSpacing + 35.f);
    window.draw(levelText);

    sf::Text linesText;
    linesText.setFont(font);
    linesText.setString("Lines  " + std::to_string(playerGrid.totalLinesCleared));
    linesText.setCharacterSize(linesSize);
    linesText.setFillColor(sf::Color::White);
    linesText.setPosition(offsetX, offsetY + (verticalSpacing * 2.f) + 35.f);
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

// initialization of the players
void Player::initializeSinglePlayer()
{
    generateTetro(currentTetro);
    generateTetro(nextTetro);
}

void Player::initializeSplitScreenPlayer(int id)
{
    this->id = id;

    generateTetro(currentTetro);
    generateTetro(nextTetro);
}