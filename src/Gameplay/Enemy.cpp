#include "Enemy.hpp"
#include <Application.hpp>
#include "AttackHitbox.hpp"
#include "Health.hpp"
#include "BloodEffect.hpp"
#include <Time.hpp>
#include <Components/Sprite.hpp>
#include <Components/Animator.hpp>
#include <Components/Collider.hpp>
#include <Components/Transform.hpp>

// generic for all melee enemies
void setMeleeAttackAnimations(Entity newEnemy, Entity atkHitbox, Animator* animator) {
    animator->addAnimation("attack_right", { 10, 0, 2, 4, 48 }, "attacks");
    animator->addAnimation("attack_left", { 10, 0, 5, 4, 48 }, "attacks");

    animator->setAnimationsTrigger(animator->animCategories.at("attacks"), [newEnemy, atkHitbox](i32 frame) {
        auto* enemy = newEnemy.get<Enemy>();
        auto* enemyTf = newEnemy.get<Transform>();
        auto* enemySprite = newEnemy.get<Sprite>();
        auto* hitbox = atkHitbox.get<AttackHitbox>();
        auto* atkHitboxCollider = atkHitbox.get<Collider>();
        auto* atkHitboxTf = atkHitbox.get<Transform>();

        if (frame == 2) {
            spawnAttackHitbox(
                enemy->direction, hitbox, atkHitboxCollider, atkHitboxTf, enemyTf, enemySprite
            );
        }
        else if (frame == 4) {
            turnOffHitbox(atkHitboxCollider);
        }
    });
}

// specific stats for this enemy type
void spawnMeleeUndead(Entity newEnemy, Entity atkHitbox, Animator* animator) {
    auto tex = newRef<sf::Texture>();
    tex->loadFromFile("../assets/textures/melee_undead.png");
    newEnemy.add<Sprite>(tex);

    auto* enemy = newEnemy.add<Enemy>(EnemyType::undeadMelee);
    enemy->attackCooldown = 100.f;
    enemy->moveSpeed = Random::rangef(80.f, 110.f);
    enemy->seeRadius = 300.f;
    enemy->forgetRadius = 400.f;

    atkHitbox.add<AttackHitbox>(30.f, 60.f, 20)->targets = AttackHitbox::player;
    atkHitbox.add<Collider>(sf::Vector2f(20.f, 20.f))->active = false;

    setMeleeAttackAnimations(newEnemy, atkHitbox, animator);
}

void spawnEnemy(Scene& scene, EnemyType type, const sf::Vector2f& position) {
    Entity newEnemy = scene.newEntity("enemy");
    Entity atkHitbox = scene.newEntity();
    Entity blood = initBloodEffect(scene);

    auto* animator = newEnemy.add<Animator>();

    switch (type) {
    case EnemyType::undeadMelee:
        spawnMeleeUndead(newEnemy, atkHitbox, animator);
        break;
    case EnemyType::undeadArcher:
        break;
    }

    newEnemy.add<Collider>(sf::Vector2f(30, 60));
    newEnemy.add<Health>([newEnemy, atkHitbox, &scene, blood](i32 newHealth, i32 newPoise) mutable {
        blood.get<BloodEffect>()->active = true;

        auto* bloodTf = blood.get<Transform>();
        auto* bloodSprite = blood.get<Sprite>();

        auto* enemy = newEnemy.get<Enemy>();
        auto* enemyTf = newEnemy.get<Transform>();
        auto* enemySprite = newEnemy.get<Sprite>();
        bloodTf->position = enemyTf->position;

        if (newHealth <= 0) {
            enemy->dying = true;
            enemy->asociatedEntities.push_back(blood);
            enemy->asociatedEntities.push_back(atkHitbox);
        }
        if (newPoise <= 0) {
            newEnemy.get<Enemy>()->staggered = true;
        }
    }, 50, 20);

    animator->addAnimation("idle_right", { 50, 0, 0, 1, 48 });
    animator->addAnimation("move_right", { 15, 0, 1, 3, 48 });
    animator->addAnimation("stan_right", { 7, 0, 6, 2, 48 });
    animator->addAnimation("die_right",  { 15, 0, 8, 2, 48 });

    animator->addAnimation("idle_left", { 50, 0, 3, 1, 48 });
    animator->addAnimation("move_left", { 15, 0, 4, 3, 48 });
    animator->addAnimation("stan_left", { 7, 0, 7, 2, 48 });
    animator->addAnimation("die_left",  { 15, 0, 9, 2, 48 });

    auto* tf = newEnemy.get<Transform>();
    tf->position = position;
    tf->scale = { 3.f, 3.f };
}

void meleeEnemyAi(Enemy* enemy, Animator* animator, Transform* tf, const Transform* playerTf) {
    f32 dist = distance(tf->position, playerTf->position);

    if (dist <= enemy->seeRadius && enemy->state != Enemy::attack) {
        enemy->state = Enemy::chase;
    }

    if (enemy->state == Enemy::chase && dist > enemy->forgetRadius) {
        enemy->state = Enemy::wander;
    }

    if (enemy->state == Enemy::chase && enemy->canAttack && dist < 30.f) {
        enemy->state = Enemy::attack;
    }

    if (!enemy->canAttack) {
        enemy->attackCdTimer += 100.f * Time::dt();
        if (enemy->attackCdTimer >= enemy->attackCooldown) {
            enemy->canAttack = true;
            enemy->attackCdTimer = 0.f;
        }
    }

    switch (enemy->state) {
    case Enemy::chase:
        if (dist < 30.f) {
            animator->play("idle_" + directionNames.at(enemy->direction));
            break;
        }

        moveTowards(tf->position, playerTf->position, enemy->moveSpeed * Time::dt());
        if (playerTf->position.x > tf->position.x) {
            enemy->direction = MoveDirection::right;
        }
        else {
            enemy->direction = MoveDirection::left;
        }
        animator->play("move_" + directionNames.at(enemy->direction));
        break;
    case Enemy::wander:
        animator->play("idle_" + directionNames.at(enemy->direction));
        break;
    case Enemy::attack:
        animator->play("attack_" + directionNames.at(enemy->direction));
        if (animator->currentAnimation->isFinished()) {
            enemy->state = Enemy::chase;
            enemy->canAttack = false;
        }
        break;
    }
}

void enemyAISystem(const std::vector<Entity>& entities) {
    for (const auto& entity : entities) {
        if (!entity.has<Enemy>() || !entity.has<Animator>()) continue;

        auto* tf = entity.get<Transform>();
        auto* enemy = entity.get<Enemy>();
        auto* animator = entity.get<Animator>();
        auto* health = entity.get<Health>();

        Entity player = findEntityByName(entities, "player");
        auto* playerTf = player.get<Transform>();
        
        if (enemy->dying) {
            Scene& scene = Application::getInstance()->scene();
            animator->play("die_" + directionNames.at(enemy->direction));
            if (animator->currentAnimation->isFinished()) {
                for (Entity& asEntt : enemy->asociatedEntities) {
                    scene.removeEntity(asEntt);
                }
                scene.removeEntity(entity);
            }
            continue;
        }

        if (enemy->staggered) {
            animator->play("stan_" + directionNames.at(enemy->direction));
            if (animator->currentAnimation->isFinished()) {
                enemy->staggered = false;
                health->poise = health->maxPoise;
            }
            continue;
        }

        switch (enemy->type) {
        case EnemyType::undeadMelee:
            meleeEnemyAi(enemy, animator, tf, playerTf);
            break;
        }
    }
}
