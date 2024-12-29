#include "game.hpp"

Game::Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Run Away From The Goblin"),
               playerPosition(GRID_WIDTH / 2 * GRID_SIZE, GRID_HEIGHT / 2 * GRID_SIZE) {
    player.setPosition(playerPosition.x, playerPosition.y);
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

void Game::handleInput(sf::Event::KeyEvent key) {
    switch (key.code) {
        case sf::Keyboard::Numpad1:
            if (playerPosition.x - GRID_SIZE >= 0 && playerPosition.y + GRID_SIZE < WINDOW_HEIGHT) {
                playerPosition.x -= GRID_SIZE;
                playerPosition.y += GRID_SIZE;
            }
            break;
        case sf::Keyboard::Numpad2:
            if (playerPosition.y + GRID_SIZE < WINDOW_HEIGHT) {
                playerPosition.y += GRID_SIZE;
            }
            break;
        case sf::Keyboard::Numpad3:
            if (playerPosition.x + GRID_SIZE < WINDOW_WIDTH && playerPosition.y + GRID_SIZE < WINDOW_HEIGHT) {
                playerPosition.x += GRID_SIZE;
                playerPosition.y += GRID_SIZE;
            }
            break;
        case sf::Keyboard::Numpad4:
            if (playerPosition.x - GRID_SIZE >= 0) {
                playerPosition.x -= GRID_SIZE;
            }
            break;
        case sf::Keyboard::Numpad5:
            break; // Numpad5 is essentially a "wait" / "pass turn" command
        case sf::Keyboard::Numpad6:
            if (playerPosition.x + GRID_SIZE < WINDOW_WIDTH) {
                playerPosition.x += GRID_SIZE;
            }
            break;
        case sf::Keyboard::Numpad7:
            if (playerPosition.x - GRID_SIZE >= 0 && playerPosition.y - GRID_SIZE >= 0) {
                playerPosition.x -= GRID_SIZE;
                playerPosition.y -= GRID_SIZE;
            }
            break;
        case sf::Keyboard::Numpad8:
            if (playerPosition.y - GRID_SIZE >= 0) {
                playerPosition.y -= GRID_SIZE;
            }
            break;
        case sf::Keyboard::Numpad9:
            if (playerPosition.x + GRID_SIZE < WINDOW_WIDTH && playerPosition.y - GRID_SIZE >= 0) {
                playerPosition.x += GRID_SIZE;
                playerPosition.y -= GRID_SIZE;
            }
            break;
    }

    player.setPosition(playerPosition.x, playerPosition.y); // Update the player's position
}

void Game::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed) {
                handleInput(event.key);
            }
        }

        window.clear();
        drawGrid();
        player.draw(window, sf::RenderStates::Default);
        window.display();
    }
}