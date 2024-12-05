#include "Tilemap.hpp"

Tilemap::Tilemap(const Ref<sf::Texture>& tex, u32 width, u32 height) 
    : texture(tex) {
    tiles.resize(height);
    for (auto& row : tiles) {
        row.resize(width);
    }
}


void Tilemap::addTile(i32 x, i32 y, Tile tile) {
    assert((y < tiles.size() && x < tiles[0].size()) && "x or y in addTile are out of bounds"); 

    tiles[x][y] = tile;
}
