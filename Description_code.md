# Description du code - Projet Tetris C++

## 1. Vue d'ensemble de l'architecture

Le projet est structuré selon une séparation logique entre le "Cœur" du jeu (logique métier) et l'Interface Utilisateur (menus, affichage). Il utilise la bibliothèque **SFML** pour le rendu graphique, l'audio et la gestion des événements.

### Arborescence
* **src/core/** : Contient la logique de jeu (`Game`, `Grid`, `Player`, `Tetros`, `Leaderboard`).
* **src/ui/** : Contient la gestion des menus et de l'interface (`Menu`).
* **assets/** : Contient les ressources (images, sons, polices).

## 2. Diagramme de Classes Simplifié

Le système repose sur un contrôleur principal (`GameController`) qui orchestre la boucle de jeu.

* **GameController** : Gère la fenêtre SFML, la boucle temporelle (TimeStep), et possède les instances de `Player`.
* **Player** : Représente un joueur. Il possède sa propre `Grid` (grille de jeu), son score, et sa pièce courante (`Tetro`). En mode multijoueur, deux instances de `Player` sont instanciées.
* **Grid** : Gère le tableau de données (le champ de jeu), la détection des collisions et la suppression des lignes.
* **Tetro** : Classe de base pour les pièces (Tetrominos). Gère la position, la couleur et la rotation.

## 3. Détails des composants clés

### 3.1. La Grille (Grid)
La grille n'est pas un tableau 2D standard, mais un **vecteur 1D** (`std::vector<int> cells`) de taille `ROWS * COLUMNS`.
* **Accès :** L'index est calculé via la formule `index = y * COLUMNS + x`.
* **États des cellules :**
    * `0` : Vide.
    * `1` : Pièce active (en mouvement).
    * `2-8` : Bloc verrouillé (la valeur correspond à la couleur de la pièce).

### 3.2. Les Pièces (Tetros)
Les pièces utilisent le système de rotation **SRS (Super Rotation System)**, standard des Tetris modernes.
* **Héritage :** Toutes les pièces (`TetroI`, `TetroT`, etc.) héritent de la classe `Tetro`.
* **Stockage :** Les 4 rotations possibles de chaque pièce sont pré-calculées dans un tableau 3D `rotations[4][4][4]`.
* **Génération (7-Bag Randomizer) :** Pour éviter les répétitions ou les "sécheresses" (longue période sans barre "I"), le jeu génère un "sac" contenant une instance de chaque pièce, le mélange, et le vide avant d'en recréer un nouveau.

### 3.3. Gestion des Collisions et Wall Kicks
La méthode `check_collision` dans `grid.cpp` teste les futurs mouvements. Si une rotation échoue (collision avec un mur), le système tente des **"Wall Kicks"** (décalages) :
1.  Le jeu teste la rotation simple.
2.  Si échec, il teste une série de décalages (droite, gauche, bas) définis par le standard SRS.
3.  Si un décalage fonctionne, la pièce est déplacée et tournée. Sinon, la rotation est annulée.

### 3.4. Le Multijoueur (Split-Screen)
Le multijoueur est géré nativement par la classe `GameController` qui contient un tableau `Player player[2]`.
* Si le mode est "Solo", seul `player[0]` est mis à jour et affiché au centre.
* Si le mode est "Split-Screen", `player[0]` et `player[1]` sont mis à jour simultanément. La méthode `setGridPosition` calcule dynamiquement l'offset d'affichage (gauche ou droite de l'écran) selon l'ID du joueur.

### 3.5. Leaderboard (Persistance)
Les scores sont sauvegardés dans un fichier texte local (`leaderboard.txt`).
* Le fichier est lu au démarrage et trié par score décroissant.
* Seuls les 10 meilleurs scores sont conservés.

## 4. Système de Build (CMake)
Le projet utilise **CMake** avec le module `FetchContent`.
* Cela permet de télécharger et compiler **SFML 2.6.1** et la librairie **GameMenu** automatiquement lors de la première compilation, garantissant que le projet fonctionne sur n'importe quelle machine disposant d'un compilateur C++ et d'une connexion internet.