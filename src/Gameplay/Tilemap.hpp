#pragma once
#include <Entity.hpp>

struct Tile {
    std::string name;
    sf::IntRect subTexture;
};

struct Tilemap {
    Tilemap(const Ref<sf::Texture>& tex, u32 width, u32 height);

    std::vector<std::vector<Tile>> tiles;
    Ref<sf::Texture> texture;

    void addTile(i32 x, i32 y, Tile tile);
};
