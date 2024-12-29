#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

#include "playerCharacter.hpp"

class Game {
private:
    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 720;

    const int GRID_SIZE = 40;
    const int GRID_WIDTH = WINDOW_WIDTH / GRID_SIZE;
    const int GRID_HEIGHT = WINDOW_HEIGHT / GRID_SIZE;

    sf::RenderWindow window;
    PlayerCharacter player;

    sf::Vector2i playerPosition;

public:
    Game();
    void run();
    void drawGrid();
    void handleInput(sf::Event::KeyEvent event);
};