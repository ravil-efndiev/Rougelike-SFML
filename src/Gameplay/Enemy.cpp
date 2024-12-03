#include "Enemy.hpp"
#include "AttackHitbox.hpp"
#include <Components/Sprite.hpp>
#include <Components/Animator.hpp>
#include <Components/Collider.hpp>
#include <Components/Transform.hpp>

void spawnEnemy(Scene& scene, EnemyType type) {
    Entity newEnemy = scene.newEntity();
    newEnemy.add<Enemy>(type);

    auto tex = newRef<sf::Texture>();
    switch (type) {
    case EnemyType::undeadMelee:
        tex->loadFromFile("../assets/textures/melee_undead.png");
        break;
    case EnemyType::undeadArcher:
        break;
    }

    newEnemy.add<Sprite>(tex);
    auto* animator = newEnemy.add<Animator>();

    Entity atkHitbox = scene.newEntity();
    atkHitbox.add<AttackHitbox>(30.f, 60.f)->targets = AttackHitbox::player;
    atkHitbox.add<Collider>(sf::Vector2f(20.f, 20.f))->debugRender = true;

    animator->addAnimation("idle_right",   { 50, 0, 0, 1, 48 });
    animator->addAnimation("move_right",   { 15, 0, 1, 3, 48 });
    animator->addAnimation("attack_right", { 10, 0, 2, 4, 48 }, "attacks");

    animator->addAnimation("idle_left",   { 50, 0, 3, 1, 48 });
    animator->addAnimation("move_left",   { 15, 0, 4, 3, 48 });
    animator->addAnimation("attack_left", { 10, 0, 5, 4, 48 }, "attacks");

    animator->setAnimationsTrigger(animator->animCategories.at("attacks"), [newEnemy, atkHitbox](i32 frame) {
        auto* enemy = newEnemy.get<Enemy>();
        auto* enemyTf = newEnemy.get<Transform>();
        auto* enemySprite = newEnemy.get<Sprite>();
        auto* hitbox = atkHitbox.get<AttackHitbox>();
        auto* atkHitboxCollider = atkHitbox.get<Collider>();
        auto* atkHitboxTf = atkHitbox.get<Transform>();
        
        const f32 horizontalWidth = 30.f;
        const f32 horizontalHeight = 60.f;

        if (frame == 2) {
            spawnAttackHitbox(
                enemy->direction, hitbox, atkHitboxCollider, atkHitboxTf, enemyTf, enemySprite
            );
        }
        else if (frame == 4) {
            turnOffHitbox(atkHitboxCollider);
        }
    });

    auto* tf = newEnemy.get<Transform>();
    tf->position = { 200.f, 200.f };
    tf->scale = { 3.f, 3.f };
}

void enemyAISystem(const std::vector<Entity>& entities) {
    for (const auto& entity : entities) {
        if (!entity.has<Enemy>() || !entity.has<Animator>()) continue;

        auto* tf = entity.get<Transform>();
        auto* enemy = entity.get<Enemy>();
        auto* animator = entity.get<Animator>();

        Entity player = findEntityByName(entities, "player");
        auto* playerTf = player.get<Transform>();
        f32 dist = distance(tf->position, playerTf->position);

        if (dist <= 200.f) {
            enemy->state = Enemy::chase;
        }

        if (enemy->state == Enemy::chase && dist > 300.f) {
            enemy->state = Enemy::wander;
        }

        animator->play("idle_left");
    }
}
