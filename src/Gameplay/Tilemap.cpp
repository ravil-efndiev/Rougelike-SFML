#include "Tilemap.hpp"

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
        return tile.position().x == pos.x && tile.position().y == pos.y;
    });

    if (it != tiles.end()) {
        *it = Tile(texture, pos, tileSubTexture, name);
    }
    else {
        tiles.push_back(Tile(texture, pos, tileSubTexture, name));
    }
}

sf::Vector2i Tilemap::simplifyPosition(const sf::Vector2f& pos) {
    std::cout << "posx: " << pos.x << " posy: " << pos.y << "\n";
    std::cout << "tileSize: " << tileSize << "\n";
    std::cout << "posx roundeed: " << (i32)roundf(pos.x / (f32)tileSize) << "\n";

    sf::Vector2f centerCoords {
        pos.x - ((f32)tileSize * 3.f) / 2.f,
        pos.y - ((f32)tileSize * 3.f) / 2.f
    };

    return {
        (i32)roundf(centerCoords.x / ((f32)tileSize * 3.f)),
        (i32)roundf(centerCoords.y / ((f32)tileSize * 3.f)),
    };
}

void createTilemap(Scene& scene) {
}
