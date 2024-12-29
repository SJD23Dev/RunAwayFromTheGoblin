#include "game.hpp"

// Functions for A* pathfinding
namespace std {
    template <>
    struct hash<sf::Vector2i> {
        std::size_t operator()(const sf::Vector2i& v) const {
            return std::hash<int>()(v.x) ^ std::hash<int>()(v.y);
        }
    };
}

struct CompareVector2i {
    bool operator()(const std::pair<int, sf::Vector2i>& a, const std::pair<int, sf::Vector2i>& b) const {
        return a.first > b.first;
    }
};
// End of A* pathfinding functions

Game::Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Run Away From The Goblin"),
               playerPosition(GRID_WIDTH / 2 * GRID_SIZE, GRID_HEIGHT / 2 * GRID_SIZE),
               goblinPosition(GRID_WIDTH / 2 * GRID_SIZE, GRID_HEIGHT / 2 * GRID_SIZE) {
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

std::vector<sf::Vector2i> Game::findPath(sf::Vector2i start, sf::Vector2i end) {
    auto heuristic = [](sf::Vector2i a, sf::Vector2i b) {
        return std::abs(a.x - b.x) + std::abs(a.y - b.y);
    };

    std::priority_queue<std::pair<int, sf::Vector2i>, std::vector<std::pair<int, sf::Vector2i>>, CompareVector2i> openSet;
    std::unordered_map<sf::Vector2i, sf::Vector2i, std::hash<sf::Vector2i>> cameFrom;
    std::unordered_map<sf::Vector2i, int, std::hash<sf::Vector2i>> gScore;
    std::unordered_map<sf::Vector2i, int, std::hash<sf::Vector2i>> fScore;

    openSet.emplace(0, start);
    gScore[start] = 0;
    fScore[start] = heuristic(start, end);

    std::vector<sf::Vector2i> directions = {
        {GRID_SIZE, 0},
        {-GRID_SIZE, 0},
        {0, GRID_SIZE},
        {0, -GRID_SIZE}
    };

    while (!openSet.empty()) {
        sf::Vector2i current = openSet.top().second;
        openSet.pop();

        if (current == end) {
            std::vector<sf::Vector2i> path;
            while (current != start) {
                path.push_back(current);
                current = cameFrom[current];
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        for (const auto& direction : directions) {
            sf::Vector2i neighbor = current + direction;
            if (neighbor.x < 0 ||
                neighbor.x >= WINDOW_WIDTH || 
                neighbor.y < 0 || 
                neighbor.y >= WINDOW_HEIGHT) {
                continue;
            }

            int tentative_gScore = gScore[current] + GRID_SIZE;
            if (gScore.find(neighbor) == gScore.end() || tentative_gScore < gScore[neighbor]) {
                cameFrom[neighbor] = current;
                gScore[neighbor] = tentative_gScore;
                fScore[neighbor] = gScore[neighbor] + heuristic(neighbor, end);
                openSet.emplace(fScore[neighbor], neighbor);
            }
        }
    }

    return {};
}

void Game::run() {
    while (window.isOpen()) {
        sf::Event event;
        bool moveHandled = false;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed) {
                handleInput(event.key, moveHandled);
            }
        }

        if (moveHandled) {
            std::vector<sf::Vector2i> path = findPath(goblinPosition, playerPosition);
            if (!path.empty()) {
                goblinPosition = path[0];
                goblin.setPosition(goblinPosition.x, goblinPosition.y);
            }

            window.clear();
            drawGrid();
            player.draw(window, sf::RenderStates::Default);
            goblin.draw(window, sf::RenderStates::Default);
            window.display();
        }
    }
}