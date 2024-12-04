#include "Game.hpp"
#include <Entity.hpp>
#include "Math.hpp"
#include "Time.hpp"
#include "Components/Sprite.hpp"
#include "Components/Transform.hpp"
#include "Components/SpriteSystems.hpp"
#include "Components/CollisionSystems.hpp"
#include "Gameplay/Player.hpp"
#include "Gameplay/AttackHitbox.hpp"
#include "Gameplay/Enemy.hpp"
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

    Random::init();

    mScene
        // for basic components
        .addSystem(spriteTransformSystem)
        .addSystem(spriteAnimationSystem)
        .addSystem(colliderPositionSystem)

        // player
        .addSystem(playerMovementSystem)
        .addSystem(playerCombatSystem)
        .addEventSystem(playerEventSystem)

        // enemy
        .addSystem(enemyAISystem)
        .addSystem(attackHitboxSystem);

    initPlayer(mScene);
    spawnEnemy(mScene, EnemyType::undeadMelee);
}

void Game::run() {
    while (mWindow->isOpen()) {
        Time::update();

        sf::Event event;
        while (mWindow->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                mWindow->close();
            }

            if (event.type == sf::Event::Resized) {
                mView->reset(sf::FloatRect(0.f, 0.f, event.size.width, event.size.height));
                mWindow->setView(*mView);
            }

            mScene.onEvent(event);
        }

        try {
            mScene.update();
        }
        catch (const std::out_of_range& err) {
            std::cerr << err.what();
            mWindow->close();
        }
        
        mWindow->clear(sf::Color(100, 100, 100, 255));
        mRenderer->render();
        mWindow->display();

        mScene.onMainLoopEnd();
    }
}

sf::RenderWindow& Game::window() {
    return *mWindow;
}

sf::View& Game::camera() {
    return *mView;
}
