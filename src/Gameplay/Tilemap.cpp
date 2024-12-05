#include "Tilemap.hpp"

Tile::Tile(const Ref<sf::Texture>& tex, const sf::Vector2i& position, const sf::IntRect& subTexture) 
    : mSprite(*tex), mPosition(position) {
    mSprite.setTextureRect(subTexture);
    mSprite.setScale({ 3.f, 3.f });
}

bool Tile::initialized() const {
    return mSprite.getTexture() != nullptr;
}

sf::Vector2i Tile::position() const {
    return mPosition;
}

sf::Vector2f Tile::realPosition() const
{
    return {
        mSprite.getGlobalBounds().width * (f32)mPosition.x,
        mSprite.getGlobalBounds().height * (f32)mPosition.y
    };
}

sf::Sprite& Tile::sprite() {
    return mSprite;
}

Tilemap::Tilemap(const Ref<sf::Texture>& tex) : texture(tex) {}

void Tilemap::setTile(i32 x, i32 y, const sf::IntRect& tileSubTexture) {
    auto it = std::find_if(tiles.begin(), tiles.end(), [x, y](const Tile& tile) {
        return tile.position().x == x && tile.position().y == y;
    });

    if (it != tiles.end()) {
        *it = Tile(texture, {x, y}, tileSubTexture);
    }
    else {
        tiles.push_back(Tile(texture, {x, y}, tileSubTexture));
    }
}

void createTilemap(Scene& scene) {
    Entity tilemap = scene.newEntity();

    Ref<sf::Texture> tex = newRef<sf::Texture>();
    tex->loadFromFile("../assets/textures/player_sheet.png");
    auto* tlm = tilemap.add<Tilemap>(tex);
    tlm->setTile(1, 1, sf::IntRect(48 * 2, 48 * 3, 48, 48));
    tlm->setTile(2, 1, sf::IntRect(48 * 1, 48 * 2, 48, 48));
}
