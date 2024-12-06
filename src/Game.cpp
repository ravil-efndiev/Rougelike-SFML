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
#include "Gameplay/Tilemap.hpp"
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

Game* Game::sInstance {};

Game* Game::create() {
    sInstance = new Game();
    return sInstance;
}

Game* Game::getInstance() {
    return sInstance;
}

Game::Game() {
    sf::VideoMode winSize = R_DEBUG ? sf::VideoMode(800, 600) : sf::VideoMode();
    u32 winStyle = R_DEBUG ? sf::Style::None : sf::Style::Fullscreen;
    mWindow = newPtr<sf::RenderWindow>(winSize, "game", winStyle);
    mView = newPtr<sf::View>(mWindow->getDefaultView());
    mRenderer = newPtr<SceneRenderer>(mScene, *mWindow);
    mWindow->setView(*mView);
    mWindow->setFramerateLimit(60);

    Random::init();
    (void)ImGui::SFML::Init(*mWindow);

    mScene
        .addSystem(spriteTransformSystem)
        .addSystem(spriteAnimationSystem)
        .addSystem(colliderPositionSystem)

        .addSystem(playerMovementSystem)
        .addSystem(playerCombatSystem)
        .addEventSystem(playerEventSystem)
        .addSystem(enemyAISystem)
        .addSystem(attackHitboxSystem);

    initPlayer(mScene);
    for (i32 i = 0; i < 5; i++)
        spawnEnemy(mScene, EnemyType::undeadMelee, {Random::rangef(300.f, 600.f), Random::rangef(10.f, 300.f)});
}

void Game::run() {
    while (mWindow->isOpen()) {
        Time::update();

        sf::Event event;
        while (mWindow->pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) {
                mWindow->close();
            }

            if (event.type == sf::Event::Resized) {
                mView->reset(sf::FloatRect(0.f, 0.f, event.size.width, event.size.height));
                mWindow->setView(*mView);
            }

            mScene.onEvent(event);
        }

        mScene.update();
        ImGui::SFML::Update(*mWindow, Time::sfDt());

        ImGui::Begin("Test");
        ImGui::Text("Sigma");
        ImGui::End();

        mWindow->clear(sf::Color(100, 100, 100, 255));
        mRenderer->render();
        ImGui::SFML::Render(*mWindow);
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
