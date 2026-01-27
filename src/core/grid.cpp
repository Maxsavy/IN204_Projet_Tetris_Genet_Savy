#include "grid.hpp"
#include "tetros.hpp"
#include <iostream>

/* Fonction de test de l'affichage de la grille dans le terminal 
void Grid::display_terminal() const
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            std::cout << cells[i * columns + j] << " ";
        }
        std::cout << std::endl;
    }
}
*/

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
        locking = false;
        return true;
    }
    else if (move_count <= 0)
    {
        update_with_tetro(tetro, tetro.colorRef);
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
                
                // Check collision avec pièce en dessous
                if (cibleY + 1 < rows)
                {
                    int indx_bottom = (cibleY + 1) * columns + cibleX;
                    if (cells[indx_bottom] != 0 && cells[indx_bottom] != 1)
                    {
                        if (cibleY < 1)
                            return 3; // Collision avec le plafond
                        start_locking_timer();
                        return 2; // Collision avec une pièce en dessous
                    }
                }

                // Check collision latérale avec pièce lockée
                int indx = cibleY * columns + cibleX;
                if (indx >= 0 && indx < rows * columns)
                {
                    if (cells[indx] != 0 && cells[indx] != 1)
                    {
                        return 1; // Collision avec une pièce lockée sur les côtés
                    }
                }
            }   
        }
    }
    return 0; // Pas de collision
}

/*
int Grid::check_collision(const Tetro &tetro, const std::array<std::array<int, 4>, 4> &shape, int futureX, int futureY)
{
    // fonction de gestion des collisions pour les 4 cas suivantts:
    // 0: pas de collision
    // 1: collision mur gauche/droite ou piece verouillée
    // 2: collision sol ou piece verouillée en descendant
    // 3: collision plafond 

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (shape[i][j] == 1)
            {
                int cibleX = futureX + j;
                int cibleY = futureY + i;

                // Check des collisions
                

                if (cibleX < 0 || cibleX >= columns)
                {
                    return 1; // Collisions avec les murs
                }

                if (cibleY >= rows)
                {
                    start_locking_timer();
                    return 2; // Collision avec le sol
                }
                
                int indx_bottom = (tetro.position.y +1 + i) * columns + cibleX;
                if (cells[indx_bottom] != 0 && cells[indx_bottom] != 1)
                {
                    if (tetro.position.y - 1 < 1)
                        return 3;      // Collision avec le plafond
                    start_locking_timer();
                        return 2; // Collision avec le sol
                    
                }

                int indx = cibleY * columns + cibleX;
                if (cells[indx] != 0 && cells[indx] != 1)
                {
                    return 1; // Collision avec une pièce lockée sur les côtés
                }

                
            }   
        }
    }
    return 0; // No collision
}
*/


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
    for (const auto& kick : kickTests)
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
    for (int i = 0; i < rows * columns; i++)
    {
        if (cells[i] == 1)
            cells[i] = 0;
    }

    const auto &shape = tetro.getShape(0);
    Tetro ghostTetro = tetro;
    // Move the ghost tetro down until collision
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
                    cells[idx] = state;
                }
            }
        }
    }
}

void Grid::delete_full_rows(int &nbDestroyedLines)
{
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
            nbDestroyedLines++;
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
}

void Grid::drawGrid(sf::RenderWindow &window, const Tetro &currentTetro)
{
    this->update_with_tetro(currentTetro, 1);
    int rows = this->rows-2;
    int cols = this->columns;
    int cellSize = this->cellSize;
    float pixelSize = static_cast<float>(cellSize * RESIZE_FACTOR);
    float targetW = static_cast<float>(cols) * pixelSize;
    float targetH = static_cast<float>(rows) * pixelSize;
    float originX = (static_cast<float>(window.getSize().x) - targetW) / 2.f;
    float originY = (static_cast<float>(window.getSize().y) - targetH) / 2.f;

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
            int idx = j + (i+2) * cols;
            int val = 0;
            if (idx >= 0 && idx < static_cast<int>(this->cells.size()))
                val = this->cells[idx];

            if (val == 0)
                cellShape.setFillColor(sf::Color::Black);
            else if (val == 1)
                cellShape.setFillColor(currentTetro.color); // active piece with its color
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
