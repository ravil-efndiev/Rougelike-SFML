#pragma once
#include <include.hpp>

struct Sprite {
    Sprite(const Ref<sf::Texture>& texture, const sf::Color& color = sf::Color::White) 
        : texture(texture) {
        sprite.setTexture(*texture);
        sprite.setColor(color);
    }

    Ref<sf::Texture> texture;
    sf::Sprite sprite;

    sf::Vector2f centerPosition(const sf::Vector2f& topleft) {
        return {
            topleft.x + sprite.getGlobalBounds().width / 2.f,
            topleft.y + sprite.getGlobalBounds().height / 2.f
        };
    }
};
