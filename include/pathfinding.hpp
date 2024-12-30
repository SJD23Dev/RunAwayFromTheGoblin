#pragma once

#include <vector>
#include <queue>
#include <unordered_map>

#include <SFML/Graphics.hpp>

class Pathfinding {
public:
    struct CompareVector2i;
    std::vector<sf::Vector2i> findPath(sf::Vector2i start, sf::Vector2i end, 
                                       int gridSize, int windowWidth, int windowHeight);
};