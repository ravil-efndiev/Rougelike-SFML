#pragma once
#include <Scene.hpp>
#include <Entity.hpp>

class Tile {
public:
    Tile() = default;
    explicit Tile(
        const Ref<sf::Texture>& tex,
        const sf::Vector2i& position,
        const sf::IntRect& subTexture
    );

    bool initialized() const;
    sf::Vector2i position() const;
    sf::Vector2f realPosition() const;
    sf::Sprite& sprite();

private:
    sf::Sprite mSprite;
    sf::Vector2i mPosition;
};

struct Tilemap {
    Tilemap(const Ref<sf::Texture>& tex);

    std::vector<Tile> tiles;
    Ref<sf::Texture> texture;

    void setTile(i32 x, i32 y, const sf::IntRect& tileSubTexture);
};

void createTilemap(Scene& scene);
