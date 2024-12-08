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
    f32 seeRadius {}, forgetRadius {};
    f32 moveSpeed {};
    bool canAttack = true;
    bool staggered = false;
    bool dying = false;

    std::vector<Entity> asociatedEntities;
};

void spawnEnemy(Scene& scene, EnemyType type, const sf::Vector2f& position);

void enemyAISystem(const std::vector<Entity>& entities);
