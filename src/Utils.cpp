#include "Utils.hpp"

sf::Vector2f getMousePosition() {
    Game* app = Game::getInstance();
    sf::Vector2i mousePixel = sf::Mouse::getPosition(app->window());
    return app->window().mapPixelToCoords(mousePixel);
}
