#pragma once
#include "ECS/Scene.hpp"
#include "ECS/SceneRenderer.hpp"
#include "Components/Animator.hpp"

class Game {
public:
    static Game* create(u16 winWidth, u16 winHeight, const sf::String& winTitle);
    static Game* getInstance();

    Game(u16 winWidth, u16 winHeight, const sf::String& winTitle);
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
