#ifndef LEADERBOARD_HPP
#define LEADERBOARD_HPP

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

struct LeaderboardEntry
{
    std::string name;
    int score;
    int level;
};

class Leaderboard
{
public:
    void printPlayerInfo(const std::string &name, int level, int score);
    void printLeaderboard(const std::string &filename);
    void start(sf::RenderWindow &window);
    void addEntry(const std::string &filename, const std::string &name, int score, int level);

private:
    std::vector<LeaderboardEntry> loadLeaderboard(const std::string &filename);
    void saveLeaderboard(const std::string &filename, const std::vector<LeaderboardEntry> &entries);
};

#endif