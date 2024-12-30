#include "game.hpp"

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

    Pathfinding pathfinding;

    // Main game loop
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
            goblinPath = pathfinding.findPath(goblinPosition, playerPosition, GRID_SIZE, WINDOW_WIDTH, WINDOW_HEIGHT);
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
        window.display();
    }
}