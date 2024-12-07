#include "Application.hpp"
#include <Entity.hpp>
#include "Math.hpp"
#include "Time.hpp"
#include "MapEditorState.hpp"
#include "GameState.hpp"
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

Application* Application::sInstance {};

Application* Application::create(State state, const std::string& editorFilePath) {
    sInstance = new Application(state, editorFilePath);
    return sInstance;
}

Application* Application::getInstance() {
    return sInstance;
}

Application::Application(State state, const std::string& editorFilePath) {
    sf::VideoMode winSize = R_DEV ? sf::VideoMode(800, 600) : sf::VideoMode();
    u32 winStyle = R_DEV ? sf::Style::Default : sf::Style::Fullscreen;
    mWindow = newPtr<sf::RenderWindow>(winSize, "game", winStyle);
    mView = newPtr<sf::View>(mWindow->getDefaultView());
    mRenderer = newPtr<SceneRenderer>(mScene, *mWindow);
    mWindow->setView(*mView);
    mWindow->setFramerateLimit(60);

    Random::init();
    bool imguiInitialized = ImGui::SFML::Init(*mWindow);
    R_ASSERT(imguiInitialized, "ImGui::SFML::Init error")

    if (state == game) {
        mState = newPtr<GameState>(mScene);
    }
    else if (state == editor) {
        mState = newPtr<MapEditorState>(
            mScene,
            editorFilePath,
            "../assets/map/test_set.yml",
            "../assets/textures/tilemap.png"
        );
    }
}

void Application::run() {
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

            mState->onEvent(event);
            mScene.onEvent(event);
        }

        mState->update();
        ImGui::SFML::Update(*mWindow, Time::sfDt());

        mState->renderUI();

        mWindow->clear(sf::Color(100, 100, 100, 255));
        mRenderer->render();
        ImGui::SFML::Render(*mWindow);
        mWindow->display();

        mScene.onMainLoopEnd();
    }
}

sf::RenderWindow& Application::window() {
    return *mWindow;
}

sf::View& Application::camera() {
    return *mView;
}
