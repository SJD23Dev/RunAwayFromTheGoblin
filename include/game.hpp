#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

#include "playerCharacter.hpp"

class Game {
private:
    sf::RenderWindow window;
    PlayerCharacter player;

public:
    Game();
    void run();
};