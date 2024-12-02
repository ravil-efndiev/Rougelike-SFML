#pragma once
#include <Entity.hpp>
#include <Scene.hpp>

enum class MoveDirection {
    up, down, left, right
};

struct Player {
    const float moveSpeed = 200.f;
    MoveDirection direction = MoveDirection::down;
    MoveDirection bufferedDirection = MoveDirection::down;

    bool attack = false;
    bool bufferedAttack = false;
    bool combo = false;
    f32 comboTimer = 0;

    void resetAttack();
};

void initPlayer(Scene& scene);

void playerMovementSystem(const std::vector<Entity>& entities);
void playerCombatSystem(const std::vector<Entity>& entities);
void playerEventSystem(const sf::Event& event, const std::vector<Entity>& entities);
