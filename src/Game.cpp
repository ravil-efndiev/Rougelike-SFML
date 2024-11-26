#include "Game.hpp"

struct CMP1 {
    int a;
};

struct CMP2 {
    float b;
};

Game::Game(u16 winWidth, u16 winHeight, const sf::String& winTitle) {
    mWindow = newPtr<sf::RenderWindow>(sf::VideoMode(winWidth, winHeight), winTitle);
    auto test = mScene.newEntity();
    test.add<CMP1>(10);
    test.add<CMP2>(20);

    std::cout << test.get<CMP1>()->a << "\n";
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
