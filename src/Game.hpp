#pragma once
#include "ECS/Scene.hpp"
#include "ECS/SceneRenderer.hpp"
#include "Components/Animator.hpp"

class Game {
public:
    Game(u16 winWidth, u16 winHeight, const sf::String& winTitle);
    ~Game() = default;

    void run();

private:
    Ptr<sf::RenderWindow> mWindow;
    Scene mScene;
    Ptr<SceneRenderer> mRenderer;
    Ptr<sf::View> mView;
};
