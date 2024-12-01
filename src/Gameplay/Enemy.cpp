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
    atkHitbox.add<AttackHitbox>()->targets = AttackHitbox::player;
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
        auto* atkHitboxCollider = atkHitbox.get<Collider>();
        auto* atkHitboxTf = atkHitbox.get<Transform>();
        
        const f32 horizontalWidth = 30.f;
        const f32 horizontalHeight = 60.f;

        if (frame == 2) {
            atkHitboxCollider->active = true;
            switch (enemy->direction) {
            case MoveDirection::left:
                atkHitboxCollider->bounds.width = horizontalWidth;
                atkHitboxCollider->bounds.height = horizontalHeight;
                atkHitboxTf->position = centerColliderToSprite(atkHitboxCollider, enemyTf, enemySprite);
                atkHitboxTf->position.x -= horizontalWidth;
                break;
            case MoveDirection::right:
                atkHitboxCollider->bounds.width = horizontalWidth;
                atkHitboxCollider->bounds.height = horizontalHeight;
                atkHitboxTf->position = centerColliderToSprite(atkHitboxCollider, enemyTf, enemySprite);
                atkHitboxTf->position.x += horizontalWidth;
                break;
            default:
                break;
            }
        }
        else if (frame == 4) {
            atkHitboxCollider->active = false;
        }
    });

    auto* tf = newEnemy.get<Transform>();
    tf->position = { 200.f, 200.f };
    tf->scale = { 3.f, 3.f };
}

void enemyAISystem(const std::vector<Entity>& entities) {
    for (const auto& entity : entities) {
        if (!entity.has<Enemy>() || !entity.has<Animator>()) continue;

        auto* animator =  entity.get<Animator>();
        animator->play("attack_right");
    }
}
