#include "Game.hpp"
#include <Entity.hpp>
#include "Time.hpp"
#include "Components/Sprite.hpp"
#include "Components/Transform.hpp"
#include <SFML/Graphics.hpp>

Game::Game(u16 winWidth, u16 winHeight, const sf::String& winTitle) {
    mWindow = newPtr<sf::RenderWindow>(sf::VideoMode(winWidth, winHeight), winTitle);
    mView = newPtr<sf::View>(sf::FloatRect(0.f, 0.f, (f32)winWidth, (f32)winHeight));
    mRenderer = newPtr<SceneRenderer>(mScene, *mWindow);
    mWindow->setView(*mView);

    Entity player = mScene.newEntity();
    Ref<sf::Texture> tex = newRef<sf::Texture>();
    tex->loadFromFile("../assets/textures/player_sheet.png");
    player.add<Sprite>(tex);
    animator = player.add<Animator>();

    animator->addAnimation("idle_down", (AnimationProps) {
        .frameDuration = 40,
        .startX = 0,
        .startY = 0,
        .endX = 3,
        .subTextureSize = 48,
    });

    player.get<Transform>()->scale = {3.f, 3.f};
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
        }

        animator->play("idle_down");
        mScene.update();
        
        mWindow->clear(sf::Color::Black);
        mRenderer->render();
        mWindow->display();
    }
}
