#include "Player.hpp"
#include <Components/Transform.hpp>
#include <Components/Sprite.hpp>
#include <Components/Animator.hpp>
#include <Time.hpp>

static const std::unordered_map<std::string, MoveDirection> directions = {
    {"left", MoveDirection::left},
    {"right", MoveDirection::right},
    {"down", MoveDirection::down},
    {"up", MoveDirection::up},
};

void normalize(sf::Vector2f& vec) {
    f32 magnitude = sqrt(pow(vec.x, 2) + pow(vec.y, 2));
    if (magnitude != 0) vec /= magnitude;
}

void initPlayer(Entity player) {
    player.add<Player>();
    Ref<sf::Texture> tex = newRef<sf::Texture>();
    tex->loadFromFile("../assets/textures/player_sheet.png");
    player.add<Sprite>(tex);

    auto* animator = player.add<Animator>();
    animator->addAnimation("idle_down",  { 30, 0, 0, 3, 48 });
    animator->addAnimation("idle_right", { 30, 0, 2, 3, 48 });
    animator->addAnimation("idle_up",    { 30, 0, 4, 3, 48 });
    animator->addAnimation("idle_left",  { 30, 0, 6, 3, 48 });

    animator->addAnimation("move_down",  { 10, 0, 1, 5, 48 });
    animator->addAnimation("move_right", { 10, 0, 3, 5, 48 });
    animator->addAnimation("move_up",    { 10, 0, 5, 5, 48 });
    animator->addAnimation("move_left",  { 10, 0, 7, 5, 48 });

    player.get<Transform>()->scale = { 3.f, 3.f };
}

void playerSystem(const std::vector<Entity>& entities) {
    for (const auto& entity : entities) {
        if (!entity.has<Player>() 
            || !entity.has<Transform>() 
            || !entity.has<Animator>()) continue;

        auto* tf = entity.get<Transform>();
        auto* player = entity.get<Player>();
        auto* animator = entity.get<Animator>();

        sf::Vector2f movement(0.f, 0.f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            animator->play("move_up");
            player->direction = MoveDirection::up;
            movement.y -= 1.f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            animator->play("move_down");
            player->direction = MoveDirection::down;
            movement.y += 1.f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            animator->play("move_right");
            player->direction = MoveDirection::right;
            movement.x += 1.f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            animator->play("move_left");
            player->direction = MoveDirection::left;
            movement.x -= 1.f;
        }


        if (movement == sf::Vector2f(0.f, 0.f)) {
            for (const auto& [directionName, direction] : directions) {
                if (player->direction == direction) {
                    animator->play("idle_" + directionName);
                }
            }
        }

        normalize(movement);
        tf->position += movement * player->moveSpeed * Time::dt();
    }
}
