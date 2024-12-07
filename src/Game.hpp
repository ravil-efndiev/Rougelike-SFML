#pragma once
#include "ECS/Scene.hpp"
#include "ECS/SceneRenderer.hpp"
#include "Components/Animator.hpp"
#include "State.hpp"

class Game {
public:
    enum State {
        game, editor
    };

    static Game* create(State state);
    static Game* getInstance();

    Game(State state);
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

    Ptr<IState> mState;
};
