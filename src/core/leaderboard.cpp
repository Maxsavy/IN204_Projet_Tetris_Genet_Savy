#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "leaderboard.hpp"

// load the data from leaderboard.txt and return a vector of LeaderboardEntry structs
std::vector<LeaderboardEntry> Leaderboard::loadLeaderboard(const std::string &filename)
{
    std::vector<LeaderboardEntry> entries;
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Could not open the file: " << filename << std::endl;
        return entries;
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        std::istringstream iss(line);
        std::string name;
        int score, level;

        if (iss >> name >> score >> level)
        {
            entries.push_back({name, score, level});
        }
    }
    std::sort(entries.begin(), entries.end(),
              [](const LeaderboardEntry &a, const LeaderboardEntry &b)
              {
                  return a.score > b.score; // Higher scores first
              });
    file.close();
    return entries;
}
// function to access the leaderboard page with the best 10 scores
void Leaderboard::start(sf::RenderWindow &window)
{
    sf::Font font;
    if (!font.loadFromFile("assets/fonts/arcade.ttf"))
    {
        std::cerr << "Failed to load font: arcade.ttf" << std::endl;
        return;
    }

    sf::Texture background;
    sf::Sprite background_sprite;
    if (background.loadFromFile("assets/images/background_menu.jpg"))
    {
        background_sprite.setTexture(background);
        sf::Vector2u imageSize = background.getSize();
        if (imageSize.x > 0 && imageSize.y > 0)
        {
            float scaleX = static_cast<float>(window.getSize().x) / imageSize.x;
            float scaleY = static_cast<float>(window.getSize().y) / imageSize.y;
            background_sprite.setScale(scaleX, scaleY);
            background_sprite.setPosition(0.f, 0.f);
        }
    }

    std::vector<LeaderboardEntry> entries = loadLeaderboard("src/core/leaderboard.txt");

    // different texts displays
    sf::Text title;
    title.setFont(font);
    title.setString("LEADERBOARD");
    title.setCharacterSize(60);
    title.setFillColor(sf::Color::White);
    title.setPosition(window.getSize().x / 2.0f - 200.0f, 50.0f);

    sf::Text headerRank, headerName, headerScore, headerLevel;
    headerRank.setFont(font);
    headerName.setFont(font);
    headerScore.setFont(font);
    headerLevel.setFont(font);

    headerRank.setString("Rank");
    headerName.setString("Name");
    headerScore.setString("Score");
    headerLevel.setString("Level");

    headerRank.setCharacterSize(35);
    headerName.setCharacterSize(35);
    headerScore.setCharacterSize(35);
    headerLevel.setCharacterSize(35);

    headerRank.setFillColor(sf::Color::Yellow);
    headerName.setFillColor(sf::Color::Yellow);
    headerScore.setFillColor(sf::Color::Yellow);
    headerLevel.setFillColor(sf::Color::Yellow);

    float startX = 150.0f;
    float startY = 150.0f;

    headerRank.setPosition(startX, startY);
    headerName.setPosition(startX + 150.0f, startY);
    headerScore.setPosition(startX + 400.0f, startY);
    headerLevel.setPosition(startX + 600.0f, startY);

    sf::Text instructions;
    instructions.setFont(font);
    instructions.setString("Press ESC to return");
    instructions.setCharacterSize(25);
    instructions.setFillColor(sf::Color(200, 200, 200));
    instructions.setPosition(window.getSize().x / 2.0f - 180.0f, window.getSize().y - 60.0f);

    window.setFramerateLimit(60);

    // the page loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return;
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                return;
            }
        }

        window.clear();

        if (background_sprite.getTexture())
        {
            window.draw(background_sprite);
        }

        window.draw(title);

        window.draw(headerRank);
        window.draw(headerName);
        window.draw(headerScore);
        window.draw(headerLevel);

        float entryY = startY + 60.0f;
        int maxEntries = std::min(static_cast<int>(entries.size()), 10); // show top 10

        for (int i = 0; i < maxEntries; i++)
        {
            sf::Text rank, name, score, level;
            rank.setFont(font);
            name.setFont(font);
            score.setFont(font);
            level.setFont(font);

            rank.setString(std::to_string(i + 1));
            name.setString(entries[i].name);
            score.setString(std::to_string(entries[i].score));
            level.setString(std::to_string(entries[i].level));

            rank.setCharacterSize(30);
            name.setCharacterSize(30);
            score.setCharacterSize(30);
            level.setCharacterSize(30);

            // different colors for the top3
            sf::Color entryColor = sf::Color::White;
            if (i == 0)
                entryColor = sf::Color(255, 215, 0);
            else if (i == 1)
                entryColor = sf::Color(192, 192, 192);
            else if (i >= 2)
                entryColor = sf::Color(205, 127, 50);

            rank.setFillColor(entryColor);
            name.setFillColor(entryColor);
            score.setFillColor(entryColor);
            level.setFillColor(entryColor);

            rank.setPosition(startX + 20.0f, entryY);
            name.setPosition(startX + 150.0f, entryY);
            score.setPosition(startX + 400.0f, entryY);
            level.setPosition(startX + 620.0f, entryY);

            window.draw(rank);
            window.draw(name);
            window.draw(score);
            window.draw(level);

            entryY += 50.0f;
        }

        window.draw(instructions);

        window.display();
    }
}

// function to create a new entry to save in the leaderboard
void Leaderboard::addEntry(const std::string &filename, const std::string &name, int score, int level)
{
    std::vector<LeaderboardEntry> entries = loadLeaderboard(filename);

    entries.push_back({name, score, level});

    std::sort(entries.begin(), entries.end(),
              [](const LeaderboardEntry &a, const LeaderboardEntry &b)
              {
                  if (a.score == b.score)
                  {
                      return a.level > b.level;
                  }
                  return a.score > b.score;
              });

    if (entries.size() > 10)
    {
        entries.resize(10);
    }

    saveLeaderboard(filename, entries);
}

// function to save a new entry into leaderboard.txt
void Leaderboard::saveLeaderboard(const std::string &filename, const std::vector<LeaderboardEntry> &entries)
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Could not open file for writing: " << filename << std::endl;
        return;
    }

    for (const auto &entry : entries)
    {
        file << entry.name << " " << entry.score << " " << entry.level << "\n";
    }

    file.close();
}