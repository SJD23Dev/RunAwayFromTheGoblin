#include "game.hpp"

Game::Game() : window(sf::VideoMode(1280, 720), "Run Away From The Goblin") {
}

void Game::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed) {
                // TODO: Handle input
            }
        }

        window.clear();
        player.draw(window, sf::RenderStates::Default);
        window.display();
    }
}