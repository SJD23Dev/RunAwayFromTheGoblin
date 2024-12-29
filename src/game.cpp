#include "game.hpp"

Game::Game() : window(sf::VideoMode(1280, 720), "Run Away From The Goblin") {
}

void Game::run() {
    while (window.isOpen()) {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
    }
}