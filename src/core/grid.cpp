#include "grid.hpp"
#include "tetros.hpp"
#include <iostream>

void Grid::update_with_tetro(const Tetro &tetro, int state)
{
    for (int i = 0; i < rows * columns; i++)
    {
        if (cells[i] == 1)
            cells[i] = 0;
    }

    const auto &shape = tetro.getShape(0);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (shape[i][j] == 1)
            {
                int gridX = tetro.position.x + j;
                int gridY = tetro.position.y + i;

                if (gridX >= 0 && gridX < columns && gridY >= 0 && gridY < rows)
                {
                    int idx = gridY * columns + gridX;
                    cells[idx] = state;
                }
            }
        }
    }
}

void Grid::start_locking_timer()
{
    if (!locking)
    {
        locking = true;
        lockingDelayClock.restart();
    }
}

void Grid::cancel_locking_timer()
{
    locking = false;
}

bool Grid::locking_tetro(const Tetro &tetro, int move_count)
{

    if (locking && lockingDelayClock.getElapsedTime().asSeconds() >= 0.5f)
    {
        update_with_tetro(tetro, tetro.colorRef);
        lockSound.play();
        locking = false;
        return true;
    }
    else if (move_count <= 0)
    {
        update_with_tetro(tetro, tetro.colorRef);
        lockSound.play();
        locking = false;
        return true;
    }
    return false;
}

int Grid::check_collision(const Tetro &tetro, const std::array<std::array<int, 4>, 4> &shape, int futureX, int futureY)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (shape[i][j] == 1)
            {
                int cibleX = futureX + j;
                int cibleY = futureY + i;

                // Check horizontal bounds
                if (cibleX < 0 || cibleX >= columns)
                {
                    return 1; // Collision avec les murs
                }

                // Check vertical bounds
                if (cibleY >= rows)
                {
                    start_locking_timer();
                    return 2; // Collision avec le sol
                }

                // Vérifier collision avec cellules déjà occupées
                int indx = cibleY * columns + cibleX;
                if (cells[indx] != 0 && cells[indx] != 1)
                {
                    // Vérifier si c'est une collision latérale ou verticale
                    // En comparant avec la position actuelle
                    if (cibleY == tetro.position.y + i) // Même ligne verticale
                    {
                        return 1; // Collision latérale
                    }
                    else // Différente ligne verticale
                    {
                        if (tetro.position.y < 2)
                            return 3; // Collision avec le plafond
                        start_locking_timer();
                        return 2; // Collision par le bas
                    }
                }
            }
        }
    }
    return 0; // Pas de collision
}

std::pair<int, int> Grid::try_wall_kicks(const Tetro &tetro, const std::array<std::array<int, 4>, 4> &newShape, int currentRotation)
{
    // Positions de test standard pour le wall kick (SRS - Super Rotation System)
    // Format: {offsetX, offsetY}
    std::vector<std::pair<int, int>> kickTests;

    // Pour les pièces I, utilisez un système différent
    bool isIPiece = (tetro.colorRef == 2); // Supposant que I = colorRef 2

    if (isIPiece)
    {
        // Wall kicks spéciaux pour la pièce I
        if (currentRotation == 0 || currentRotation == 2)
        {
            kickTests = {{0, 0}, {-2, 0}, {1, 0}, {-2, -1}, {1, 2}};
        }
        else
        {
            kickTests = {{0, 0}, {-1, 0}, {2, 0}, {-1, 2}, {2, -1}};
        }
    }
    else
    {
        // Wall kicks standard pour J, L, S, T, Z
        kickTests = {
            {0, 0},   // Pas de décalage (test normal)
            {-1, 0},  // Décalage gauche
            {1, 0},   // Décalage droite
            {0, -1},  // Décalage haut
            {-1, -1}, // Diagonale haut-gauche
            {1, -1}   // Diagonale haut-droite
        };
    }

    // Tester chaque position
    for (const auto &kick : kickTests)
    {
        int testX = tetro.position.x + kick.first;
        int testY = tetro.position.y + kick.second;

        int collision = check_collision(tetro, newShape, testX, testY);

        // Si pas de collision avec mur/pièce lockée (0 ou 2 acceptable si pas au sol)
        if (collision == 0)
        {
            return kick; // Retourne le décalage qui fonctionne
        }
    }

    return {0, 0}; // Aucun kick n'a fonctionné
}

void Grid::update_with_ghost_tetro(const Tetro &tetro, int state)
{
    // Ne pas effacer les cellules 1 ici, car on veut afficher le ghost ET la pièce actuelle

    const auto &shape = tetro.getShape(0); // Utiliser la rotation actuelle
    Tetro ghostTetro = tetro;

    // Descendre le ghost tetro jusqu'à collision
    while (true)
    {
        int collision = check_collision(ghostTetro, shape, ghostTetro.position.x, ghostTetro.position.y + 1);
        if (collision == 0)
        {
            ghostTetro.moveDown(1);
        }
        else
        {
            break;
        }
    }

    // Dessiner le ghost sur la grille
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (shape[i][j] == 1)
            {
                int gridX = ghostTetro.position.x + j;
                int gridY = ghostTetro.position.y + i;

                if (gridX >= 0 && gridX < columns && gridY >= 0 && gridY < rows)
                {
                    int idx = gridY * columns + gridX;
                    // Ne dessiner le ghost que si la cellule est vide ou contient la pièce actuelle
                    if (cells[idx] == 0 || cells[idx] == 1)
                    {
                        cells[idx] = state;
                    }
                }
            }
        }
    }
}

std::vector<int> Grid::delete_full_rows()
{
    std::vector<int> fullRows;
    for (int i = 0; i < rows; i++)
    {
        bool isFull = true;
        for (int j = 0; j < columns; j++)
        {
            if (cells[i * columns + j] == 0 || cells[i * columns + j] == 1)
            {
                isFull = false;
                break;
            }
        }

        if (isFull)
        {
            fullRows.push_back(i);
            linesCleared++;
            totalLinesCleared++;
            // Move all rows above down by one
            for (int k = i; k > 0; k--)
            {
                for (int j = 0; j < columns; j++)
                {
                    cells[k * columns + j] = cells[(k - 1) * columns + j];
                }
            }
            // Clear the top row
            for (int j = 0; j < columns; j++)
            {
                cells[j] = 0;
            }
        }
    }
    return fullRows;
}

void Grid::setGridPosition(int mode, int pId, float windowWidth, float windowHeight)
{
    gameMode = mode;
    playerId = pId;

    float pixelSize = static_cast<float>(CELL_SIZE * RESIZE_FACTOR);
    float gridWidth = static_cast<float>(columns) * pixelSize;
    float gridHeight = static_cast<float>(rows - 2) * pixelSize;

    if (mode == 1)
    {
        originX = (windowWidth - gridWidth) / 2.0f;
        originY = (windowHeight - gridHeight) / 2.0f;
    }
    else if (mode == 2)
    {
        if (pId == 0)
        {
            originX = (windowWidth / 4.0f) - (gridWidth / 2.0f);
            originY = (windowHeight - gridHeight) / 2.0f;
        }
        else
        {
            originX = (3.0f * windowWidth / 4.0f) - (gridWidth / 2.0f);
            originY = (windowHeight - gridHeight) / 2.0f;
        }
    }
}

void Grid::drawGameGrid(sf::RenderWindow &window, const Tetro &tetro)
{
    this->update_with_tetro(tetro, 1);
    int visibleRows = rows - 2;
    float pixelSize = static_cast<float>(CELL_SIZE * RESIZE_FACTOR);

    drawGrid(window, tetro, visibleRows, columns, originX, originY, pixelSize);
}

void Grid::drawNextGrid(sf::RenderWindow &window, Tetro &nextTetro)
{
    float pixelSize = static_cast<float>(CELL_SIZE * RESIZE_FACTOR);
    float gridWidth = static_cast<float>(columns) * pixelSize;

    float nextGridX = originX + gridWidth + 30.0f;
    float nextGridY = originY;

    nextTetro.setPosition(1, -1);
    this->update_with_tetro(nextTetro, 1);
    drawGrid(window, nextTetro, 6, 6, nextGridX, nextGridY, pixelSize);
    nextTetro.setPosition(3, -1);
}

void Grid::drawGrid(sf::RenderWindow &window, const Tetro &tetro, int rows, const int cols, float originX, float originY, float pixelSize)
{
    sf::RectangleShape cellShape(sf::Vector2f(pixelSize, pixelSize));
    cellShape.setOutlineThickness(1.f);
    cellShape.setOutlineColor(sf::Color(100, 100, 100));

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            float x = originX + j * pixelSize;
            float y = originY + i * pixelSize;
            cellShape.setPosition(x, y);
            int rowOffset = (cols == 6) ? -2 : 2;
            int idx = j + (i + rowOffset) * this->columns;
            int val = 0;
            if (idx >= 0 && idx < static_cast<int>(this->cells.size()))
                val = this->cells[idx];

            if (val == 0)
                cellShape.setFillColor(sf::Color::Black);
            else if (val == 1)
                cellShape.setFillColor(tetro.color); // active piece with its color
            else if (val != 0 && val != 1)
            {
                switch (val)
                {
                case 2:
                    cellShape.setFillColor(sf::Color::Cyan); // I piece
                    break;
                case 3:
                    cellShape.setFillColor(sf::Color::Blue); // J piece
                    break;
                case 4:
                    cellShape.setFillColor(sf::Color(255, 165, 0)); // L piece
                    break;
                case 5:
                    cellShape.setFillColor(sf::Color::Yellow); // O piece
                    break;
                case 6:
                    cellShape.setFillColor(sf::Color::Green); // S piece
                    break;
                case 7:
                    cellShape.setFillColor(sf::Color::Magenta); // T piece
                    break;
                case 8:
                    cellShape.setFillColor(sf::Color::Red); // Z piece
                    break;
                }
            }

            window.draw(cellShape);
        }
    }
}

void Grid::drawGhostTetro(sf::RenderWindow &window, const Tetro &currentTetro)
{
    int visibleRows = rows - 2;
    float pixelSize = static_cast<float>(cellSize * RESIZE_FACTOR);

    sf::RectangleShape cellShape(sf::Vector2f(pixelSize, pixelSize));
    cellShape.setOutlineThickness(2.f);
    cellShape.setOutlineColor(currentTetro.color);

    sf::Color ghostColor = currentTetro.color;
    ghostColor.a = 80;
    cellShape.setFillColor(ghostColor);

    const auto &shape = currentTetro.getShape(0);
    Tetro ghostTetro = currentTetro;

    while (true)
    {
        int collision = check_collision(ghostTetro, shape,
                                        ghostTetro.position.x,
                                        ghostTetro.position.y + 1);
        if (collision == 0)
        {
            ghostTetro.moveDown(1);
        }
        else
        {
            break;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (shape[i][j] == 1)
            {
                int gridX = ghostTetro.position.x + j;
                int gridY = ghostTetro.position.y + i - 2;

                if (gridX >= 0 && gridX < columns && gridY >= 0 && gridY < visibleRows)
                {
                    float x = originX + gridX * pixelSize;
                    float y = originY + gridY * pixelSize;
                    cellShape.setPosition(x, y);
                    window.draw(cellShape);
                }
            }
        }
    }
}

void Grid::draw_deleted_row_animation(sf::RenderWindow &window, const std::vector<int> &deletedRow)
{
    float pixelSize = static_cast<float>(CELL_SIZE * RESIZE_FACTOR);
    float mainGridW = static_cast<float>(columns) * pixelSize;
    float mainGridH = static_cast<float>(rows - 2) * pixelSize;

    sf::RectangleShape cellShape(sf::Vector2f(pixelSize, pixelSize));
    cellShape.setOutlineThickness(1.f);
    cellShape.setOutlineColor(sf::Color(100, 100, 100));

    for (int step = 0; step < static_cast<int>(pixelSize); step += 4)
    {
        for (int j = 0; j < columns; ++j)
        {
            for (int row : deletedRow)
            {
                // Effacer de l'intérieur vers l'extérieur
                float xLeft = originX + j * pixelSize + step / 2.f;
                float y = originY + (row - 2) * pixelSize; // -2 pour l'offset des lignes cachées
                cellShape.setPosition(xLeft, y);
                cellShape.setSize(sf::Vector2f(pixelSize - step, pixelSize));
                cellShape.setFillColor(sf::Color::Black);
                window.draw(cellShape);
            }
        }
        lineClearSound.play();
        window.display();
        sf::sleep(sf::milliseconds(30));
    }
}