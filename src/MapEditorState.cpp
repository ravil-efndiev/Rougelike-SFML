#include "MapEditorState.hpp"
#include <yaml-cpp/yaml.h>
#include <imgui.h>
#include <imgui-SFML.h>
#include <Utils.hpp>

MapEditorState::MapEditorState(Scene& scene, const std::string& tilesetPath, const std::string& texturePath) 
        : mTilesetPath(tilesetPath), mScene(scene) {
    loadTileset();

    Ref<sf::Texture> tex = newRef<sf::Texture>();
    tex->loadFromFile(texturePath);

    mTilemapEntt = mScene.newEntity("tilemap");
    mTilemap = mTilemapEntt.add<Tilemap>(tex, mTileSize);
}

void MapEditorState::loadTileset() {
    YAML::Node tileset = YAML::LoadFile(mTilesetPath);    

    YAML::Node tileSize = tileset["tileSize"];
    R_ASSERT(tileSize, "Tileset doesnt contain tileSize field")
    mTileSize = tileSize.as<i32>();

    YAML::Node tiles = tileset["tiles"];
    R_ASSERT(tileSize, "Tileset doesnt contain tiles array")

    for (const auto& tileEntry : tiles) {
        std::string tileName = tileEntry.first.as<std::string>();
        YAML::Node texCoords = tileEntry.second;

        R_ASSERT((texCoords.IsSequence() && texCoords.size() == 2), "invalid tex coords data in tileset file")
        R_ASSERT((mTileset.find(tileName) == mTileset.end()), "tile names must be unique in a set")

        mTileset.emplace(tileName, sf::Vector2i(texCoords[0].as<i32>(), texCoords[1].as<i32>()));
        mTilesetInsertions.push_back(tileName);
    }
}

void MapEditorState::update() {
    if (ImGui::GetIO().WantCaptureMouse) return;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2i mousePos = mTilemap->simplifyPosition(getMousePosition());
        
        auto it = std::find_if(mTilemap->tiles.begin(), mTilemap->tiles.end(), 
            [mousePos](const Tile& tile) {
                return tile.position() == mousePos;
            }
        );

        if (it != mTilemap->tiles.end()) {
            mSelectedTile = it->name();
        }
    }
    else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2i mousePos = mTilemap->simplifyPosition(getMousePosition());
            
        if (!mSelectedTile.empty()) {
            auto coords = mTileset.at(mSelectedTile);
            mTilemap->setTile(mousePos, coordsToIntRect(coords), mSelectedTile);
        }
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        sf::Vector2i mousePos = mTilemap->simplifyPosition(getMousePosition());

        mTilemap->setTile(mousePos, sf::IntRect(0, 0, 0, 0), "");
    }

    mScene.update();
}

void MapEditorState::renderUI() {
    ImGui::Begin("Tile Pallete");

    float padding = 12.f;
    float imageSize = 50.f;
    float cellSize = padding + imageSize;

    ImGui::Columns(ImGui::GetContentRegionAvail().x / cellSize, nullptr, false);

    for (const auto& tileName : mTilesetInsertions) {
        sf::Sprite buttonSprite (*mTilemap->texture);
        buttonSprite.setTextureRect(coordsToIntRect(mTileset.at(tileName)));
        if (ImGui::ImageButton(tileName.c_str(), buttonSprite, {imageSize, imageSize})) {
            mSelectedTile = tileName;
        }
        ImGui::NextColumn();
    }
    ImGui::Columns(1);
    ImGui::End();
}

sf::IntRect MapEditorState::coordsToIntRect(const sf::Vector2i& coords) {
    return sf::IntRect(coords.x * mTileSize, coords.y * mTileSize, mTileSize, mTileSize);
}
