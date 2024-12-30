#include "game.hpp"

// Functions for A* pathfinding
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
struct CompareVector2i {
    bool operator()(const std::pair<int, sf::Vector2i>& a, const std::pair<int, sf::Vector2i>& b) const {
        // Compare based on the fScore (first element of the pair)
        return a.first > b.first;
    }
};
// End of A* pathfinding functions

Game::Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Run Away From The Goblin"),
               rng(std::random_device{}()) {
    sf::Image icon;
    if (!icon.loadFromFile("../../../assets/images/icon.png")) {
        throw std::runtime_error("Game::Game() - Unable to load icon from file!");
    }
    window.setIcon(32, 32, icon.getPixelsPtr());

    std::uniform_int_distribution<> disX(0, GRID_WIDTH - 1);
    std::uniform_int_distribution<> disY(0, GRID_HEIGHT - 1);

    playerPosition = sf::Vector2i(disX(rng) * GRID_SIZE, disY(rng) * GRID_SIZE);
    goblinPosition = sf::Vector2i(disX(rng) * GRID_SIZE, disY(rng) * GRID_SIZE);

    player.setPosition(playerPosition.x, playerPosition.y);
    goblin.setPosition(goblinPosition.x, goblinPosition.y);
}

void Game::drawGrid() {
    // Create a line to represent the grid
    sf::RectangleShape line(sf::Vector2f(1, WINDOW_HEIGHT));
    line.setFillColor(sf::Color(195, 183, 183, 175));

    // Draw vertical lines
    for (int i = 0; i < GRID_WIDTH; i++) {
        line.setPosition(i * GRID_SIZE, 0);
        window.draw(line);
    }

    // Draw horizontal lines
    line.setSize(sf::Vector2f(WINDOW_WIDTH, 1));
    for (int i = 0; i < GRID_HEIGHT; i++) {
        line.setPosition(0, i * GRID_SIZE);
        window.draw(line);
    }
}

void Game::handleInput(sf::Event::KeyEvent key, bool& flag) {
    switch (key.code) {
        case sf::Keyboard::Numpad0:
            highlightPath = !highlightPath;
            break;
        case sf::Keyboard::Numpad1:
            if (playerPosition.x - GRID_SIZE >= 0 && playerPosition.y + GRID_SIZE < WINDOW_HEIGHT) {
                playerPosition.x -= GRID_SIZE;
                playerPosition.y += GRID_SIZE;
            }
            flag = true;
            break;
        case sf::Keyboard::Numpad2:
            if (playerPosition.y + GRID_SIZE < WINDOW_HEIGHT) {
                playerPosition.y += GRID_SIZE;
            }
            flag = true;
            break;
        case sf::Keyboard::Numpad3:
            if (playerPosition.x + GRID_SIZE < WINDOW_WIDTH && playerPosition.y + GRID_SIZE < WINDOW_HEIGHT) {
                playerPosition.x += GRID_SIZE;
                playerPosition.y += GRID_SIZE;
            }
            flag = true;
            break;
        case sf::Keyboard::Numpad4:
            if (playerPosition.x - GRID_SIZE >= 0) {
                playerPosition.x -= GRID_SIZE;
            }
            flag = true;
            break;
        case sf::Keyboard::Numpad5:
            flag = true;
            break; // Numpad5 is essentially a "wait" / "pass turn" command
        case sf::Keyboard::Numpad6:
            if (playerPosition.x + GRID_SIZE < WINDOW_WIDTH) {
                playerPosition.x += GRID_SIZE;
            }
            flag = true;
            break;
        case sf::Keyboard::Numpad7:
            if (playerPosition.x - GRID_SIZE >= 0 && playerPosition.y - GRID_SIZE >= 0) {
                playerPosition.x -= GRID_SIZE;
                playerPosition.y -= GRID_SIZE;
            }
            flag = true;
            break;
        case sf::Keyboard::Numpad8:
            if (playerPosition.y - GRID_SIZE >= 0) {
                playerPosition.y -= GRID_SIZE;
            }
            flag = true;
            break;
        case sf::Keyboard::Numpad9:
            if (playerPosition.x + GRID_SIZE < WINDOW_WIDTH && playerPosition.y - GRID_SIZE >= 0) {
                playerPosition.x += GRID_SIZE;
                playerPosition.y -= GRID_SIZE;
            }
            flag = true;
            break;
    }

    player.setPosition(playerPosition.x, playerPosition.y); // Update the player's position
}

void Game::handleMouseInput(sf::Event::MouseButtonEvent event, bool& flag) {
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    sf::Vector2i gridPosition = {mousePosition.x / GRID_SIZE * GRID_SIZE, 
                                 mousePosition.y / GRID_SIZE * GRID_SIZE};

    switch (event.button) {
        case sf::Mouse::Button::Right:
            if (!isWall(gridPosition)) {
                auto wall = std::make_unique<Wall>(gridPosition);
                walls.push_back(std::move(wall));
                flag = true;
            }
            break;
        case sf::Mouse::Button::Left:
            walls.erase(std::remove_if(walls.begin(), walls.end(), [gridPosition](const auto& wall) {
                return wall->getPosition() == gridPosition;
            }), walls.end());
            flag = true;
            break;
        case sf::Mouse::Button::Middle:
            playerPosition = gridPosition;
            player.setPosition(playerPosition.x, playerPosition.y);
            break;
    }
}

bool Game::isWall(sf::Vector2i position) const {
    for (const auto& wall : walls) {
        if (wall->getPosition() == position) {
            return true;
        }
    }
    return false;
}

std::vector<sf::Vector2i> Game::findPath(sf::Vector2i start, sf::Vector2i end) {
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
        {GRID_SIZE, 0},
        {-GRID_SIZE, 0},
        {0, GRID_SIZE},
        {0, -GRID_SIZE},
        {GRID_SIZE, GRID_SIZE},
        {GRID_SIZE, -GRID_SIZE},
        {-GRID_SIZE, GRID_SIZE},
        {-GRID_SIZE, -GRID_SIZE}
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
                neighbor.x >= WINDOW_WIDTH || 
                neighbor.y < 0 || 
                neighbor.y >= WINDOW_HEIGHT) {
                continue;
            }

            if (isWall(neighbor)) {
                continue;
            }

            // Calculate tentative gScore for the neighbor
            int tentative_gScore = gScore[current] + GRID_SIZE;
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

void Game::drawGoblinPath() {
    if (!highlightPath || goblinPath.empty()) {
        return;
    }

    sf::RectangleShape pathRect(sf::Vector2f(GRID_SIZE, GRID_SIZE));
    pathRect.setFillColor(sf::Color(255, 255, 0, 50));

    for (const auto& position : goblinPath) {
        pathRect.setPosition(position.x, position.y);
        window.draw(pathRect);
    }
}

void Game::run() {
    // Initial draw
    window.clear();
    drawGrid();
    player.draw(window, sf::RenderStates::Default);
    goblin.draw(window, sf::RenderStates::Default);
    window.display();

    // Main game loop
    while (window.isOpen()) {
        sf::Event event;
        bool moveHandled = false;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed) {
                handleInput(event.key, moveHandled);
            } else if (event.type == sf::Event::MouseButtonPressed) {
                handleMouseInput(event.mouseButton, moveHandled);
            }
        }

        if (moveHandled) {
            goblinPath = findPath(goblinPosition, playerPosition);
            if (!goblinPath.empty()) {
                goblinPosition = goblinPath[0];
                goblin.setPosition(goblinPosition.x, goblinPosition.y);
            }
        }

        window.clear();
        drawGrid();
        drawGoblinPath();
        player.draw(window, sf::RenderStates::Default);
        goblin.draw(window, sf::RenderStates::Default);

        for (const auto& wall : walls) {
            wall->draw(window, sf::RenderStates::Default);
        }

        window.display();
    }
}