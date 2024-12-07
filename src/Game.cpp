#include "Game.hpp"
#include <Entity.hpp>
#include "Math.hpp"
#include "Time.hpp"
#include "MapEditorState.hpp"
#include "GameState.hpp"
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

Game* Game::sInstance {};

Game* Game::create(State state) {
    sInstance = new Game(state);
    return sInstance;
}

Game* Game::getInstance() {
    return sInstance;
}

Game::Game(State state) {
    sf::VideoMode winSize = R_DEBUG ? sf::VideoMode(800, 600) : sf::VideoMode();
    u32 winStyle = R_DEBUG ? sf::Style::Default : sf::Style::Fullscreen;
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
        mState = newPtr<MapEditorState>(mScene, "../assets/map/test_set.yml", "../assets/textures/tilemap.png");
    }
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

sf::RenderWindow& Game::window() {
    return *mWindow;
}

sf::View& Game::camera() {
    return *mView;
}
