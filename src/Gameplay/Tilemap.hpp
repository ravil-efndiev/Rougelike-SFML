#pragma once
#include <Scene.hpp>
#include <Entity.hpp>

class Tile {
public:
    Tile() = default;
    explicit Tile(
        const Ref<sf::Texture>& tex,
        const sf::Vector2i& position,
        const sf::IntRect& subTexture,
        const std::string& name
    );

    bool initialized() const;
    sf::Vector2i position() const;
    sf::Vector2f realPosition() const;
    std::string name() const;
    sf::Sprite& sprite();

private:
    sf::Sprite mSprite;
    sf::Vector2i mPosition;
    std::string mName;
};

struct Tilemap {
    Tilemap(const Ref<sf::Texture>& tex, u32 tileSize);

    std::vector<Tile> tiles;
    Ref<sf::Texture> texture;
    u32 tileSize;

    void setTile(const sf::Vector2i& pos, const sf::IntRect& tileSubTexture, const std::string& name);
    sf::Vector2i simplifyPosition(const sf::Vector2f& pos);
};

void createTilemap(Scene& scene);
