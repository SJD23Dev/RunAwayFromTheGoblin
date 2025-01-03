#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <set>
#include <random>

#include <SFML/Graphics.hpp>

#include "playerCharacter.hpp"
#include "goblin.hpp"
#include "pathfinding.hpp"

class Game {
private:
    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 720;

    const int GRID_SIZE = 40;
    const int GRID_WIDTH = WINDOW_WIDTH / GRID_SIZE;
    const int GRID_HEIGHT = WINDOW_HEIGHT / GRID_SIZE;

    sf::RenderWindow window;
    std::mt19937 rng;

    bool moveHandled = false;
    bool highlightPath = false;

    PlayerCharacter player;
    Goblin goblin;

    sf::Vector2i playerPosition;
    sf::Vector2i goblinPosition;
    std::vector<sf::Vector2i> goblinPath;

    void handleInput(sf::Event::KeyEvent event, bool& flag);
    void drawGrid();
    void drawGoblinPath();

public:
    Game();
    void run();
};