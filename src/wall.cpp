#include "wall.hpp"

Wall::Wall(sf::Vector2i position) {
    structure.setSize(sf::Vector2f(GRID_SIZE, GRID_SIZE));
    structure.setPosition(position.x, position.y);
    structure.setFillColor(sf::Color(120, 80, 0, 255));
}

void Wall::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(structure, states);
}

sf::Vector2i Wall::getPosition() {
    return sf::Vector2i(structure.getPosition().x, structure.getPosition().y);
}