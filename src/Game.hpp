#pragma once
#include "ECS/Scene.hpp"
#include "ECS/SceneRenderer.hpp"
#include "Components/Animator.hpp"

class Game {
public:
    static Game* create();
    static Game* getInstance();

    Game();
    ~Game() {}

    void run();

    sf::RenderWindow& window();
    sf::View& camera();

private:
    static Game* sInstance;

    Ptr<sf::RenderWindow> mWindow;
    Ptr<SceneRenderer> mRenderer;
    Ptr<sf::View> mView;
    Scene mScene;
};
