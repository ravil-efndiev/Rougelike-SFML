#include "Player.hpp"
#include <Components/Transform.hpp>
#include <Components/Sprite.hpp>
#include <Components/Animator.hpp>
#include <Time.hpp>

static const std::unordered_map<std::string, std::pair<MoveDirection, sf::Vector2f>> directions = {
    {"left",  {MoveDirection::left,  { -1.f, 0.f }}},
    {"right", {MoveDirection::right, { 1.f, 0.f }}},
    {"down",  {MoveDirection::down,  { 0.f, 1.f }}},
    {"up",    {MoveDirection::up,    { 0.f, -1.f }}},
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

    animator->addAnimation("attack_right1", { 7, 0, 8, 5, 48 });
    animator->addAnimation("attack_right2", { 7, 6, 8, 10, 48 });

    player.get<Transform>()->scale = { 3.f, 3.f };
}

void move(MoveDirection dir, Player* player, Animator* animator, sf::Vector2f& movement) {
    player->direction = dir;
    for (const auto& [directionName, direction] : directions) {
        if (direction.first == dir) {
            movement += direction.second;
            animator->play("move_" + directionName);
        }
    }
}

void playerMovementSystem(const std::vector<Entity>& entities) {
    for (const auto& entity : entities) {
        if (!entity.has<Player>() 
            || !entity.has<Transform>() 
            || !entity.has<Animator>()) continue;

        auto* tf = entity.get<Transform>();
        auto* player = entity.get<Player>();
        auto* animator = entity.get<Animator>();

        sf::Vector2f movement(0.f, 0.f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            move(MoveDirection::up, player, animator, movement);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            move(MoveDirection::down, player, animator, movement);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            move(MoveDirection::right, player, animator, movement);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            move(MoveDirection::left, player, animator, movement);
        }

        if (movement == sf::Vector2f(0.f, 0.f)) {
            for (const auto& [directionName, direction] : directions) {
                if (player->direction == direction.first) {
                    animator->play("idle_" + directionName);
                }
            }
        }

        normalize(movement);
        tf->position += movement * player->moveSpeed * Time::dt();
    }
}

void playerCombatSystem(const std::vector<Entity>& entities) {
    for (const auto& entity : entities) {
        if (!entity.has<Player>() 
            || !entity.has<Transform>() 
            || !entity.has<Animator>()) continue;   
        
        auto* player = entity.get<Player>();
        auto* animator = entity.get<Animator>();

        if (player->attack && !player->combo) {
            animator->play("attack_right1");
            if (animator->currentAnimation->isFinished()) {
                player->attack = false;
                player->combo = true;
            }
        }

        if (player->combo) {
            player->comboTimer += 100.f * Time::dt();
            if (player->attack || player->bufferedAttack) {
                animator->play("attack_right2");
                if (animator->currentAnimation->isFinished()) {
                    player->resetAttack();
                }
            }
            else if (player->comboTimer >= 100.f) {
                player->combo = false;
                player->comboTimer = 0.f;
            }
        }
    }
}

void playerEventSystem(const sf::Event& event, const std::vector<Entity>& entities) {
    if (event.type == sf::Event::MouseButtonPressed) {
        for (const auto& entity : entities) {
            if (!entity.has<Player>()) continue;

            auto* player = entity.get<Player>();
            if (player->attack) {
                player->bufferedAttack = true;
            }
            else {
                player->attack = true;
            }
        }
    }
}

void Player::resetAttack() {
    attack = false;
    bufferedAttack = false;
    combo = false;
    comboTimer = 0.f;
}
