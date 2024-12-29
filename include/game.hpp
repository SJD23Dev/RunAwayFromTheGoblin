#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <set>

#include <SFML/Graphics.hpp>

#include "playerCharacter.hpp"
#include "goblin.hpp"

class Game {
private:
    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 720;

    const int GRID_SIZE = 40;
    const int GRID_WIDTH = WINDOW_WIDTH / GRID_SIZE;
    const int GRID_HEIGHT = WINDOW_HEIGHT / GRID_SIZE;

    sf::RenderWindow window;

    bool moveHandled = false;

    PlayerCharacter player;
    Goblin goblin;

    sf::Vector2i playerPosition;
    sf::Vector2i goblinPosition;

    void handleInput(sf::Event::KeyEvent event, bool& flag);
    void drawGrid();
    std::vector<sf::Vector2i> findPath(sf::Vector2i start, sf::Vector2i end);

public:
    Game();
    void run();
};