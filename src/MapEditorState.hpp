#pragma once
#include "ECS/Scene.hpp"
#include "ECS/SceneRenderer.hpp"
#include "ECS/Entity.hpp"
#include "Gameplay/Tilemap.hpp"
#include "State.hpp"

class MapEditorState : public IState {
public:
    MapEditorState(Scene& scene, const std::string& tilesetPath, const std::string& texturePath);
    ~MapEditorState() = default;

    void update() override;
    void renderUI() override;

private:
    void loadTileset();
    sf::IntRect coordsToIntRect(const sf::Vector2i& coords);

private:
    std::string mTilesetPath;
    i32 mTileSize;

    std::unordered_map<std::string, sf::Vector2i> mTileset;
    std::vector<std::string> mTilesetInsertions;
    std::string mSelectedTile;

    Entity mTilemapEntt;
    Tilemap* mTilemap;

    Scene& mScene;
};
