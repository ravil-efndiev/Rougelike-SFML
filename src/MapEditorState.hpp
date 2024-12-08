#pragma once
#include "ECS/Scene.hpp"
#include "ECS/SceneRenderer.hpp"
#include "ECS/Entity.hpp"
#include "Gameplay/Tilemap.hpp"
#include "State.hpp"

struct SetTileData {
    SetTileData(const sf::Vector2i& subTexCoords, bool hasCollision) 
        : subTexCoords(subTexCoords), hasCollision(hasCollision) {}
        
    sf::Vector2i subTexCoords;
    bool hasCollision;
};

class MapEditorState : public IState {
public:
    MapEditorState(
        Scene& scene,
        const std::string& tilemapPath, 
        const std::string& tilesetPath, 
        const std::string& texturePath
    );
    ~MapEditorState() = default;

    void update() override;
    void renderUI() override;
    void onEvent(const sf::Event& event) override;

private:
    void loadTileset();
    sf::IntRect coordsToIntRect(const sf::Vector2i& coords);

private:
    std::string mTilesetPath;
    std::string mTilemapPath;
    i32 mTileSize;

    std::unordered_map<std::string, SetTileData> mTileset;
    std::vector<std::string> mTilesetInsertions;
    std::string mSelectedTile;

    Entity mTilemapEntt;
    Tilemap* mTilemap;

    Scene& mScene;
};
