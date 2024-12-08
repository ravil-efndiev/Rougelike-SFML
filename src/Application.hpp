#pragma once
#include "ECS/Scene.hpp"
#include "ECS/SceneRenderer.hpp"
#include "Components/Animator.hpp"
#include "State.hpp"

class Application {
public:
    enum State {
        game, editor
    };

    static Application* create(State state, const std::string& editorFilePath = "");
    static Application* getInstance();

    Application(State state, const std::string& editorFilePath);
    ~Application() {}

    void run();

    sf::RenderWindow& window();
    sf::View& camera();
    Scene& scene();

private:
    static Application* sInstance;

    Ptr<sf::RenderWindow> mWindow;
    Ptr<SceneRenderer> mRenderer;
    Ptr<sf::View> mView;
    Scene mScene;

    Ptr<IState> mState;
};
