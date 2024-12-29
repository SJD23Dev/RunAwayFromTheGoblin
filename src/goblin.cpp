#include "goblin.hpp"

Goblin::Goblin() {
    std::cout << "Goblin::Goblin() - Current working directory: " 
              << std::filesystem::current_path() << std::endl;
    if (!font.loadFromFile("../../../assets/fonts/Audex-Regular.ttf")) {
        throw std::runtime_error("Goblin::Goblin() - Unable to load font from file!");
    }

    character.setFont(font);
    character.setString("G");
    character.setCharacterSize(24);
    character.setFillColor(sf::Color::Green);
}

void Goblin::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(character, states);
}

void Goblin::setPosition(int x, int y) {
    this->x = x;
    this->y = y;
    sf::FloatRect bounds = character.getLocalBounds();
    character.setPosition(x + (GRID_SIZE / 2) - (bounds.width / 2), 
                          y + (GRID_SIZE / 2) - (bounds.height / 2)
    );
}