#pragma once
#include <include.hpp>

struct Sprite {
    Sprite(const Ref<sf::Texture>& texture, const sf::Color& color = sf::Color::Red) 
        : texture(texture) {
        sprite.setTexture(*texture);
        sprite.setColor(color);
    }

    Ref<sf::Texture> texture;
    sf::Sprite sprite;
};
