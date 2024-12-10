#include "Tilemap.hpp"
#include "ColliderList.hpp"
#include <yaml-cpp/yaml.h>

Tile::Tile(
    const Ref<sf::Texture>& tex,
    const sf::Vector2i& position,
    const sf::IntRect& subTexture,
    const std::string& name,
    bool hasCollision
) : mSprite(*tex), mPosition(position),
    mName(name), mCollision(hasCollision) 
{
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

std::string Tile::name() const { return mName; }
bool Tile::hasCollision() const { return mCollision; }
sf::Sprite& Tile::sprite() { return mSprite; }

Tilemap::Tilemap(const Ref<sf::Texture>& tex, u32 tileSize) : texture(tex), tileSize(tileSize) {}

Tilemap::Layer* Tilemap::findLayer(const std::string& name) {
    auto it = std::find_if(layers.begin(), layers.end(), [name](const Layer& layer) {
        return layer.name == name;
    });

    if (it != layers.end()) {
        return &(*it);
    }
    return nullptr;
}

Tilemap::Layer& Tilemap::getLayer(const std::string& name) {
    Layer* layer = findLayer(name);
    R_ASSERT(layer, "tilemap layer with name `" + name + "` not found");
    return *layer;
}

void Tilemap::setTile(
    const sf::Vector2i& pos, const sf::IntRect& tileSubTexture,
    const std::string& layerName, const std::string& name, bool hasCollision
) {
    Layer& layer = getLayer(layerName);
    auto it = std::find_if(layer.tiles.begin(), layer.tiles.end(), [pos](const Tile& tile) {
        return tile.position() == pos;
    });

    if (it != layer.tiles.end()) {
        *it = Tile(texture, pos, tileSubTexture, name, hasCollision);
    }
    else {
        layer.tiles.push_back(Tile(texture, pos, tileSubTexture, name, hasCollision));
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

void Tilemap::removeTile(const sf::Vector2i& pos, const std::string& layerName) {
    Layer& layer = getLayer(layerName);
    layer.tiles.erase(
        std::remove_if(layer.tiles.begin(), layer.tiles.end(), [pos](const Tile& tile) {
            return tile.position() == pos;
        }), 
        layer.tiles.end()
    );
}

void Tilemap::saveToFile(const std::string& path) {
    YAML::Emitter emitter;
    emitter << YAML::BeginMap;
    emitter << YAML::Key << "tiles" << YAML::Value;
    emitter << YAML::BeginSeq;

    for (auto& layer : layers) {
        for (auto& tile : layer.tiles) {
            sf::IntRect subtex = tile.sprite().getTextureRect();

            emitter << YAML::BeginMap;
            emitter << YAML::Key << "name" << YAML::Value << tile.name();
            emitter << YAML::Key << "layer" << YAML::Value << layer.name;
            emitter << YAML::Key << "x" << YAML::Value << tile.position().x;
            emitter << YAML::Key << "y" << YAML::Value << tile.position().y;
            emitter << YAML::Key << "texX" << YAML::Value << subtex.left;
            emitter << YAML::Key << "texY" << YAML::Value << subtex.top;
            emitter << YAML::Key << "width" << YAML::Value << subtex.width;
            emitter << YAML::Key << "height" << YAML::Value << subtex.height;
            emitter << YAML::Key << "collision" << YAML::Value << tile.hasCollision();
            emitter << YAML::EndMap;
        }
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
            tileData["name"] && tileData["x"] && tileData["y"] && 
            tileData["texX"] && tileData["texY"] &&
            tileData["width"] && tileData["height"] && 
            tileData["collision"] && tileData["layer"],
            "data missing from tile entry"
        )

        std::string tileName = tileData["name"].as<std::string>();
        std::string layerName = tileData["layer"].as<std::string>();
        bool collision = tileData["collision"].as<bool>();

        Layer* layer = findLayer(layerName);
        if (!layer) {
            layers.push_back({{}, layerName});
        }

        sf::IntRect subTexture (
            tileData["texX"].as<i32>(), tileData["texY"].as<i32>(),
            tileData["width"].as<i32>(), tileData["height"].as<i32>()
        );

        sf::Vector2i pos (tileData["x"].as<i32>(), tileData["y"].as<i32>());
        setTile(pos, subTexture, layerName, tileName, collision);
    }
}

std::vector<Tile> Tilemap::allCollisionTiles() const {
    std::vector<Tile> tiles;
    for (const auto& layer : layers) {
        for (const auto& tile : layer.tiles) {
            if (tile.hasCollision()) tiles.push_back(tile);
        }
    }
    return tiles;
}

void createTilemap(Scene& scene) {
    Entity mapEntt = scene.newEntity();
    Ref<sf::Texture> tex = newRef<sf::Texture>();
    tex->loadFromFile("../assets/textures/tilemap.png");

    auto* tlm = mapEntt.add<Tilemap>(tex, 24);
    tlm->loadFromFile("../assets/map/test_map.yml");

    auto* collList = mapEntt.add<ColliderList>(false);
    auto collTiles = tlm->allCollisionTiles();
    for (Tile& tile : collTiles) {
        if (!tile.hasCollision()) continue;

        f32 collSize = tile.sprite().getGlobalBounds().width;
        collList->colliders.push_back(Collider({collSize, collSize}, tile.realPosition(), false));
    }
}
