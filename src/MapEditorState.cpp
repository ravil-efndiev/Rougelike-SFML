#include "MapEditorState.hpp"
#include "Math.hpp"
#include "Time.hpp"
#include <yaml-cpp/yaml.h>
#include <imgui.h>
#include <imgui-SFML.h>
#include <Utils.hpp>

MapEditorState::MapEditorState(
    Scene& scene,
    const std::string& tilemapPath, 
    const std::string& tilesetPath, 
    const std::string& texturePath
) : mTilemapPath(tilemapPath), mTilesetPath(tilesetPath), mScene(scene) {
    loadTileset();

    Ref<sf::Texture> tex = newRef<sf::Texture>();
    tex->loadFromFile(texturePath);

    mTilemapEntt = mScene.newEntity("tilemap");
    mTilemap = mTilemapEntt.add<Tilemap>(tex, mTileSize);

    std::ifstream tlmFile (mTilemapPath);
    if (tlmFile.is_open()) {
        YAML::Node tlmData = YAML::LoadFile(mTilemapPath);
        if (tlmData["tiles"]) {
            mTilemap->loadFromFile(mTilemapPath);
            mCurrentLayer = mTilemap->layers[0].name;
        }
    }
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
        YAML::Node tileData = tileEntry.second;

        R_ASSERT(tileData.IsSequence() && tileData.size() == 3, "invalid tex coords data in tileset file")
        R_ASSERT(mTileset.find(tileName) == mTileset.end(), "tile names must be unique in a set")

        bool hasCollision = tileData[2].as<bool>();

        mTileset.emplace(tileName, SetTileData({tileData[0].as<i32>(), tileData[1].as<i32>()}, hasCollision));
        mTilesetInsertions.push_back(tileName);
    }
}

void updateCamera() {
    Application* app = Application::getInstance();
    sf::Vector2f cameraMoveVec;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        cameraMoveVec.y -= 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        cameraMoveVec.y += 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        cameraMoveVec.x += 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        cameraMoveVec.x -= 1.f;
    }
    normalize(cameraMoveVec);
    app->camera().move(cameraMoveVec * 400.f * Time::dt());
    app->window().setView(app->camera());
}

void MapEditorState::update() {
    if (ImGui::GetIO().WantCaptureMouse) return;
    updateCamera();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2i mousePos = mTilemap->simplifyPosition(getMousePosition());
        
        auto layer = mTilemap->getLayer(mCurrentLayer);
        auto it = std::find_if(layer.tiles.begin(), layer.tiles.end(), 
            [mousePos](const Tile& tile) {
                return tile.position() == mousePos;
            }
        );

        if (it != layer.tiles.end()) {
            mSelectedTile = it->name();
        }
    }
    else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2i mousePos = mTilemap->simplifyPosition(getMousePosition());
            
        if (!mSelectedTile.empty()) {
            auto tileData = mTileset.at(mSelectedTile);
            mTilemap->setTile(
                mousePos, coordsToIntRect(tileData.subTexCoords),
                mCurrentLayer, mSelectedTile, tileData.hasCollision
            );
        }
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        sf::Vector2i mousePos = mTilemap->simplifyPosition(getMousePosition());

        mTilemap->removeTile(mousePos, mCurrentLayer);
    }

    mScene.update();
}

void MapEditorState::onEvent(const sf::Event& event) {
    if (event.type != sf::Event::KeyPressed) return;

    if (event.key.code == sf::Keyboard::S && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
        mTilemap->saveToFile(mTilemapPath);
    }
}

#define MAX_BUFFER_SIZE 256

struct {
    char layerName[MAX_BUFFER_SIZE];
} UIData;

void MapEditorState::renderUI() {
    ImGui::Begin("Tile Pallete");

    float padding = 12.f;
    float imageSize = 50.f;
    float cellSize = padding + imageSize;

    ImGui::Columns(ImGui::GetContentRegionAvail().x / cellSize, nullptr, false);

    for (const auto& tileName : mTilesetInsertions) {
        sf::Sprite buttonSprite (*mTilemap->texture);
        buttonSprite.setTextureRect(coordsToIntRect(mTileset.at(tileName).subTexCoords));
        if (ImGui::ImageButton(tileName.c_str(), buttonSprite, {imageSize, imageSize})) {
            mSelectedTile = tileName;
        }
        ImGui::NextColumn();
    }
    ImGui::Columns(1);
    ImGui::End();

    ImGui::Begin("Layers");
    for (const auto& layer : mTilemap->layers) {
        if (ImGui::Button(layer.name.c_str())) {
            mCurrentLayer = layer.name;
        }
        if (layer.name == mCurrentLayer) {
            ImGui::SameLine();
            ImGui::TextColored({0, 255, 0, 255}, "<");
        }
    }
    ImGui::Separator();
    ImGui::Text("Add new layer");
    ImGui::InputText("name", UIData.layerName, MAX_BUFFER_SIZE);
    if (ImGui::Button("+") && strlen(UIData.layerName) > 0) {
        mTilemap->layers.push_back({{}, std::string(UIData.layerName)});
        strncpy(UIData.layerName, "", MAX_BUFFER_SIZE);
    }
    ImGui::End();
}

sf::IntRect MapEditorState::coordsToIntRect(const sf::Vector2i& coords) {
    return sf::IntRect(coords.x * mTileSize, coords.y * mTileSize, mTileSize, mTileSize);
}
