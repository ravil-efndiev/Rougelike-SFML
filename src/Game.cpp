#include "Game.hpp"

Game::Game(u16 winWidth, u16 winHeight, const sf::String& winTitle) {
    mWindow = newPtr<sf::RenderWindow>(sf::VideoMode(winWidth, winHeight), winTitle);
}

void Game::run() {
    sf::Texture tex;
    tex.loadFromFile("assets/img.png");
    sf::Sprite sprite (tex);
    sprite.setScale(0.5, 0.5);
    sprite.setColor(sf::Color::Red);

    while (mWindow->isOpen()) {
        sf::Event event;
        while (mWindow->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                mWindow->close();
            }
        }
        
        mWindow->clear(sf::Color::Black);
        mWindow->draw(sprite);
        mWindow->display();
    }
}
