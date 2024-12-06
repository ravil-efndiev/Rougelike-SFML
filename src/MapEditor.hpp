#pragma once
#include "ECS/Scene.hpp"
#include "ECS/SceneRenderer.hpp"

class MapEditor {
public:
    MapEditor(const std::string& tilesetPath);
    ~MapEditor() = default;

private:
    Ptr<sf::RenderWindow> mWindow;
    Ptr<SceneRenderer> mRenderer;
    Ptr<sf::View> mView;
    Scene mScene;
    std::string mTilesetPath;
};
