#pragma once
#include <Entity.hpp>
#include <Scene.hpp>
#include "Math.hpp"

#define PLAYER_DAMAGE_COLLIDER 0
#define PLAYER_PHYSICS_COLLIDER 1

struct Player {
    f32 moveSpeed = 200.f;
    MoveDirection direction = MoveDirection::down;
    MoveDirection bufferedDirection = MoveDirection::down;

    bool attack = false;
    bool bufferedAttack = false;
    bool combo = false;
    f32 comboTimer = 0;

    sf::Vector2f nextPositionDelta;

    void resetAttack();
};

void initPlayer(Scene& scene);

void playerMovementSystem(const std::vector<Entity>& entities);
void playerCameraSystem(const std::vector<Entity>& entities);
void playerCombatSystem(const std::vector<Entity>& entities);
void playerEventSystem(const sf::Event& event, const std::vector<Entity>& entities);
void playerCollisionSystem(const std::vector<Entity>& entities);
