#include "Utils.hpp"

sf::Vector2f getMousePosition() {
    Application* app = Application::getInstance();
    sf::Vector2i mousePixel = sf::Mouse::getPosition(app->window());
    return app->window().mapPixelToCoords(mousePixel);
}
