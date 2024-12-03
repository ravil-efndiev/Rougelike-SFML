#pragma once
#include "Player.hpp"
#include "Math.hpp"

enum class EnemyType {
    undeadMelee,
    undeadArcher,
};

struct Enemy {
    Enemy(EnemyType type) : type(type) {}
    EnemyType type;
    MoveDirection direction = MoveDirection::right;

    enum State {
        wander, chase, attack
    };

    State state = wander;
    f32 attackCooldown {}, attackCdTimer {};
    f32 moveSpeed {};
    bool canAttack = true;
};

void spawnEnemy(Scene& scene, EnemyType type);

void enemyAISystem(const std::vector<Entity>& entities);
