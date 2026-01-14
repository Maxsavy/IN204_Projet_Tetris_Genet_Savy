#include <iostream>
#include "player.hpp"

void Player::generateTetro( Tetro &tempTetro)
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

    tempTetro.setPosition(COLUMNS / 2 - 2, 0);
}

void Player::lockTetroInGrid(const Tetro &tetro)
{
    playerGrid.update_with_tetro(tetro, tetro.colorRef);
    generateTetro(currentTetro);
}