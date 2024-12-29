#pragma once

#include <iostream>
#include <filesystem>

#include <SFML/Graphics.hpp>

class Goblin : sf::Drawable{
private:
    const int GRID_SIZE = 40;

    sf::Font font;
    sf::Text character;
    int x, y;

public:
    Goblin();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void setPosition(int x, int y);
};