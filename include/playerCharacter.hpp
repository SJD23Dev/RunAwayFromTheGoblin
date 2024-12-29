#pragma once

#include <iostream>
#include <filesystem>

#include <SFML/Graphics.hpp>

class PlayerCharacter : sf::Drawable {
private:
    const int GRID_SIZE = 40;

    sf::Font font;
    sf::Text character;
    int x, y;

public:
    PlayerCharacter();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void setPosition(int x, int y);
};