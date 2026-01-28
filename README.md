# Retro Tetris

![Language](https://img.shields.io/badge/language-C++17-blue.svg)
![Library](https://img.shields.io/badge/library-SFML%202.6.1-green.svg)
![Status](https://img.shields.io/badge/status-Playable-brightgreen.svg)

**Projet de Programmation Orienté objet du cours IN204/ENSTA réalisé en C++ avec la librairie SFML.**
Ce Projet reprends les concepts de POO étudiés lors du cours IN204 affin de réimplanter toutes les mécaniques du jeu classique tetris en ajoutant un mode multijoueur en split screen.

##  Réalisé par 
* **Maxime SAVY**
* **Tiago GENET**

##  Installation et Compilation

Ce projet utilise **CMake**, a télécharger au lien suivant:
```
   https://cmake.org/download/
```
Il téléchargera automatiquement les dépendances nécessaires dont la SFML.
Si il ne parvient pas à récupérer SFML, il faudra alors installer SFML 2.6.1 sur ce lien

```
   https://www.sfml-dev.org/download/sfml/2.6.1/
```

### Prérequis
* Un compilateur C++ compatible C++17 (GCC, Clang, MSVC).
* CMake (version 3.28 ou supérieure recommandée).
* Une connexion internet (pour le premier build uniquement qui permet de récupére les librairies de menu).
* *Sous Linux :* Installez les dépendances SFML (`sudo apt install libxrandr-dev libxcursor-dev libudev-dev libfreetype-dev libflac-dev libvorbis-dev libgl1-mesa-dev`).

### Instructions (Ligne de commande)

1.  **Cloner le projet** :
    ```bash
    git clone git@github.com:Maxsavy/IN204_Projet_Tetris_Genet_Savy.git
    cd Owner IN204_Projet_Tetris_Genet_Savy

    ```

2.  **Configurer le projet** :
    ```bash
    cmake -S . -B build
    ```

3.  **Compiler** :
    ```bash
    cmake --build build --target tetris
    ```

4.  **Lancer le jeu** :
    * *Linux / Mac :* `./build/bin/tetris`
    * *Windows :* `.\build\bin\Debug\tetris.exe`


## Liste des Fonctionnalités

* **Deux modes de jeux différents:**
   * **Mode Solo** : Le classique. Marquez des points en complétant des lignes. La vitesse augmente avec les niveaux toutes les 10 lignes.
   * **Mode Multijoueur Local** : Écran scindé (Split-Screen) pour affronter un ami sur le même clavier.
* **Des mécaniques fidèles au jeu de base** :
    * **Ghost Piece** : Permet de visualisez où la pièce va tomber.
    * **7-Bag Randomizer** : Gestion de l'appartion aléatoire des pièces (comme dans les jeux officiels).
    * **Wall Kicks (SRS)** : Possibilité de faire tourner les pièces même contre les murs avec un repositionement dans la grille.
    * **Lock Delay** : Un court délai avant que la pièce ne se bloque au sol, permettant des ajustements de dernière seconde (15 ajustements max comme dans le jeu).
    * **Next Piece** : affichage de la prochaine pièce à tomber
    * **Accélération** : Augmentation de la difficulté du jeu en augmentant la vitesse
* **Leaderboard** : Sauvegarde locale des 10 meilleurs scores.
* **Audio & Musique** : Effets sonores et musiques d'ambiance.

## Les Contrôles

### Dans le Menu
* **Flèches Haut/Bas** ou **Z/S** : Naviguer dans les menus.
* **Entrée** : Valider.
* **Échap** : Quitter ou Retour.

### En Jeu

| Action | Joueur 1 (Gauche) | Joueur 2 (Droite) |
| :--- | :---: | :---: |
| **Déplacement Gauche** | `Q` | `Flèche Gauche` |
| **Déplacement Droite** | `D` | `Flèche Droite` |
| **Rotation** | `Z` | `Flèche Haut` |
| **Soft Drop (Bas)** | `S` | `Flèche Bas` |
| **Pause** | `Échap` | - |


## La Structure du Projet

```text
IN204_Projet_Tetris_Genet_Savy/
├── assets/         # Images, polices et sons
├── src/
│   ├── core/       # Logique du jeu (Grille, Pièces, Joueur)
│   ├── ui/         # Interface utilisateur (Menus)
│   └── main.cpp    # Point d'entrée
├── CMakeLists.txt  # Configuration de compilation
└── README.md       # Ce fichier