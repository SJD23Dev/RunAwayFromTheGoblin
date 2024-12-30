#pragma once

#include <SFML/Graphics.hpp>

class Wall : sf::Drawable {
private:
    const int GRID_SIZE = 40;
    sf::RectangleShape structure;

public:
    Wall(sf::Vector2i position);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    sf::Vector2i getPosition();
};