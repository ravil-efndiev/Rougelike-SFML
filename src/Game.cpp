#include "Game.hpp"
#include "Entity.hpp"
#include "Components/Sprite.hpp"
#include "Components/Transform.hpp"
#include <SFML/Graphics.hpp>

Game::Game(u16 winWidth, u16 winHeight, const sf::String& winTitle) {
    mWindow = newPtr<sf::RenderWindow>(sf::VideoMode(winWidth, winHeight), winTitle);
    mRenderer = newPtr<SceneRenderer>(mScene, *mWindow);

    Entity test = mScene.newEntity();
    Ref<sf::Texture> tex = newRef<sf::Texture>();
    tex->loadFromFile("../assets/img.png");
    test.add<Sprite>(Sprite(tex));
    test.add<Transform>({{20, 0}, {0.5, 0.5}, 45});
}

void Game::run() {
    while (mWindow->isOpen()) {
        sf::Event event;

        while (mWindow->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                mWindow->close();
            }
        }

        mScene.update();
        
        mWindow->clear(sf::Color::Black);
        mRenderer->render();
        mWindow->display();
    }
}
