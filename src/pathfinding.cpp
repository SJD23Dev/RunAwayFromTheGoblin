// Functions for A* pathfinding
#include "pathfinding.hpp"

namespace std {
    template <>
    // Custom hash function for sf::Vector2i to use it as a key in unordered_map
    struct hash<sf::Vector2i> {
        std::size_t operator()(const sf::Vector2i& v) const {
            // Combine the hash values of x and y using XOR to create a unique hash for sf::Vector2i
            return std::hash<int>()(v.x) ^ std::hash<int>()(v.y);
        }
    };
}

// Comparator for priority queue to compare pairs of (fScore, sf::Vector2i)
struct Pathfinding::CompareVector2i {
    bool operator()(const std::pair<int, sf::Vector2i>& a, const std::pair<int, sf::Vector2i>& b) const {
        // Compare based on the fScore (first element of the pair)
        return a.first > b.first;
    }
};

std::vector<sf::Vector2i> Pathfinding::findPath(sf::Vector2i start, sf::Vector2i end, int gridSize, int windowWidth, int windowHeight) {
    // Heuristic function storing the Manhattan distance between two points
    auto heuristic = [](sf::Vector2i a, sf::Vector2i b) {
        return std::abs(a.x - b.x) + std::abs(a.y - b.y);
    };

    // Initialize the open set (priority queue), cameFrom map, gScore map, and fScore map
    std::priority_queue<std::pair<int, sf::Vector2i>, std::vector<std::pair<int, sf::Vector2i>>, CompareVector2i> openSet;
    std::unordered_map<sf::Vector2i, sf::Vector2i, std::hash<sf::Vector2i>> cameFrom;
    std::unordered_map<sf::Vector2i, int, std::hash<sf::Vector2i>> gScore;
    std::unordered_map<sf::Vector2i, int, std::hash<sf::Vector2i>> fScore;

    // Add the start position to the open set with a score of 0
    openSet.emplace(0, start);
    gScore[start] = 0;
    fScore[start] = heuristic(start, end);

    // Define the possible movement directions (8 directions)
    std::vector<sf::Vector2i> directions = {
        {gridSize, 0},
        {-gridSize, 0},
        {0, gridSize},
        {0, -gridSize},
        {gridSize, gridSize},
        {gridSize, -gridSize},
        {-gridSize, gridSize},
        {-gridSize, -gridSize}
    };

    // A* algorithm
    while (!openSet.empty()) {
        // // Get the node with the lowest fScore
        sf::Vector2i current = openSet.top().second;
        openSet.pop();

        // // If the end node is reached, reconstruct the path
        if (current == end) {
            std::vector<sf::Vector2i> path;
            while (current != start) {
                path.push_back(current);
                current = cameFrom[current];
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        // Explore neighbors
        for (const auto& direction : directions) {
            sf::Vector2i neighbor = current + direction;
            // Skip out-of-bounds neighbors
            if (neighbor.x < 0 ||
                neighbor.x >= windowWidth || 
                neighbor.y < 0 || 
                neighbor.y >= windowHeight) {
                continue;
            }

            // Calculate tentative gScore for the neighbor
            int tentative_gScore = gScore[current] + gridSize;
            // If this path to the neighbor is better, record it
            if (gScore.find(neighbor) == gScore.end() || tentative_gScore < gScore[neighbor]) {
                cameFrom[neighbor] = current;
                gScore[neighbor] = tentative_gScore;
                fScore[neighbor] = gScore[neighbor] + heuristic(neighbor, end);
                openSet.emplace(fScore[neighbor], neighbor);
            }
        }
    }

    // Return an empty path if no path is found
    return {};
}