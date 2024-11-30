#include "Game.hpp"
#include <Entity.hpp>
#include "Time.hpp"
#include "Components/Sprite.hpp"
#include "Components/Transform.hpp"
#include "Components/SpriteSystems.hpp"
#include "Gameplay/Player.hpp"
#include <SFML/Graphics.hpp>

Game* Game::sInstance {};

Game* Game::create(u16 winWidth, u16 winHeight, const sf::String& winTitle) {
    sInstance = new Game(winWidth, winHeight, winTitle);
    return sInstance;
}

Game* Game::getInstance() {
    return sInstance;
}

Game::Game(u16 winWidth, u16 winHeight, const sf::String& winTitle) {
    mWindow = newPtr<sf::RenderWindow>(sf::VideoMode(winWidth, winHeight), winTitle);
    mView = newPtr<sf::View>(sf::FloatRect(0.f, 0.f, (f32)winWidth, (f32)winHeight));
    mRenderer = newPtr<SceneRenderer>(mScene, *mWindow);
    mWindow->setView(*mView);
    mWindow->setFramerateLimit(60);

    mScene
        .addSystem(spriteTransformSystem)
        .addSystem(spriteAnimationSystem)
        .addSystem(playerMovementSystem)
        .addSystem(playerCombatSystem)
        .addEventSystem(playerEventSystem);

    Entity player = mScene.newEntity();
    initPlayer(player);
}

void Game::run() {
    while (mWindow->isOpen()) {
        Time::update();
        sf::Event event;

        while (mWindow->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                std::cout << "closestart\n";
                mWindow->close();
                std::cout << "closeend\n";
            }

            if (event.type == sf::Event::Resized) {
                mView->reset(sf::FloatRect(0.f, 0.f, event.size.width, event.size.height));
                mWindow->setView(*mView);
            }

            mScene.onEvent(event);
        }

        mScene.update();
        
        mWindow->clear(sf::Color::Black);
        mRenderer->render();
        mWindow->display();
    }
}

sf::RenderWindow& Game::window() {
    return *mWindow;
}

sf::View& Game::camera() {
    return *mView;
}
