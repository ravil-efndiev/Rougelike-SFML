#include "Tilemap.hpp"
#include <yaml-cpp/yaml.h>

Tile::Tile(
    const Ref<sf::Texture>& tex,
    const sf::Vector2i& position,
    const sf::IntRect& subTexture,
    const std::string& name
) : mSprite(*tex), mPosition(position), mName(name) {
    mSprite.setTextureRect(subTexture);
    mSprite.setScale({ 3.f, 3.f });
}

bool Tile::initialized() const {
    return mSprite.getTexture() != nullptr;
}

sf::Vector2i Tile::position() const {
    return mPosition;
}

sf::Vector2f Tile::realPosition() const {
    return {
        mSprite.getGlobalBounds().width * (f32)mPosition.x,
        mSprite.getGlobalBounds().height * (f32)mPosition.y
    };
}

std::string Tile::name() const {
    return mName;
}

sf::Sprite& Tile::sprite() {
    return mSprite;
}

Tilemap::Tilemap(const Ref<sf::Texture>& tex, u32 tileSize) : texture(tex), tileSize(tileSize) {}

void Tilemap::setTile(const sf::Vector2i& pos, const sf::IntRect& tileSubTexture, const std::string& name) {
    auto it = std::find_if(tiles.begin(), tiles.end(), [pos](const Tile& tile) {
        return tile.position() == pos;
    });

    if (it != tiles.end()) {
        *it = Tile(texture, pos, tileSubTexture, name);
    }
    else {
        tiles.push_back(Tile(texture, pos, tileSubTexture, name));
    }
}

sf::Vector2i Tilemap::simplifyPosition(const sf::Vector2f& pos) {
    sf::Vector2f centerCoords {
        pos.x - ((f32)tileSize * 3.f) / 2.f,
        pos.y - ((f32)tileSize * 3.f) / 2.f
    };

    return {
        (i32)roundf(centerCoords.x / ((f32)tileSize * 3.f)),
        (i32)roundf(centerCoords.y / ((f32)tileSize * 3.f)),
    };
}

void Tilemap::removeTile(const sf::Vector2i& pos) {
    tiles.erase(
        std::remove_if(tiles.begin(), tiles.end(), [pos](const Tile& tile) {
            return tile.position() == pos;
        }), tiles.end()
    );
}


void Tilemap::saveToFile(const std::string& path) {
    YAML::Emitter emitter;
    emitter << YAML::BeginMap;
    emitter << YAML::Key << "tiles" << YAML::Value;
    emitter << YAML::BeginSeq;

    for (Tile& tile : tiles) {
        sf::IntRect subtex = tile.sprite().getTextureRect();

        emitter << YAML::BeginMap;
        emitter << YAML::Key << "name" << YAML::Value << tile.name();
        emitter << YAML::Key << "x" << YAML::Value << tile.position().x;
        emitter << YAML::Key << "y" << YAML::Value << tile.position().y;
        emitter << YAML::Key << "texX" << YAML::Value << subtex.left;
        emitter << YAML::Key << "texY" << YAML::Value << subtex.top;
        emitter << YAML::Key << "width" << YAML::Value << subtex.width;
        emitter << YAML::Key << "height" << YAML::Value << subtex.height;
        emitter << YAML::EndMap;
    }

    emitter << YAML::EndSeq << YAML::EndMap;

    std::ofstream file (path);
    file << emitter.c_str();
}

void Tilemap::loadFromFile(const std::string& path) {
    YAML::Node data = YAML::LoadFile(path);
    YAML::Node tilesSeq = data["tiles"];
    R_ASSERT(tilesSeq, "tilemap doesnt have tiles array")

    for (const auto& tileData : tilesSeq) {
        R_ASSERT(
            (tileData["name"] && tileData["x"] && tileData["y"] && 
            tileData["texX"] && tileData["texY"] &&
            tileData["width"] && tileData["height"]),
            "data missing from tile entry"
        )

        sf::IntRect subTexture (
            tileData["texX"].as<i32>(), tileData["texY"].as<i32>(),
            tileData["width"].as<i32>(), tileData["height"].as<i32>()
        );

        sf::Vector2i pos (tileData["x"].as<i32>(), tileData["y"].as<i32>());
        setTile(pos, subTexture, tileData["name"].as<std::string>());
    }
}
