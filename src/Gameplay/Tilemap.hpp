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
        const std::string& name,
        bool hasCollision
    );

    bool initialized() const;
    sf::Vector2i position() const;
    sf::Vector2f realPosition() const;
    std::string name() const;
    bool hasCollision() const;
    sf::Sprite& sprite();

private:
    sf::Sprite mSprite;
    sf::Vector2i mPosition;
    std::string mName;
    bool mCollision;
};

struct Tilemap {
    Tilemap(const Ref<sf::Texture>& tex, u32 tileSize);

    struct Layer {
        std::vector<Tile> tiles;
        std::string name;
    };

    std::vector<Layer> layers;
    Ref<sf::Texture> texture;
    u32 tileSize;
    
    Layer& getLayer(const std::string& name);
    Layer* findLayer(const std::string& name);
    std::vector<Tile> allCollisionTiles() const;

    void setTile(
        const sf::Vector2i& pos, const sf::IntRect& tileSubTexture,
        const std::string& layerName, const std::string& name, bool hasCollision
    );
    void removeTile(const sf::Vector2i& pos, const std::string& layerName);
    sf::Vector2i simplifyPosition(const sf::Vector2f& pos);
    void saveToFile(const std::string& path);
    void loadFromFile(const std::string& path);
};

void createTilemap(Scene& scene);
