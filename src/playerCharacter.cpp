#include "playerCharacter.hpp"

PlayerCharacter::PlayerCharacter() {
    std::cout << "PlayerCharacter::PlayerCharacter() - Current working directory: " 
              << std::filesystem::current_path() << std::endl;
    if (!font.loadFromFile("../assets/fonts/Audex-Regular.ttf")) {
        throw std::runtime_error("PlayerCharacter::PlayerCharacter() - Unable to load font from file!");
    }

    character.setFont(font);
    character.setString("@");
    character.setCharacterSize(24);
    character.setFillColor(sf::Color::White);
}

void PlayerCharacter::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(character, states);
}