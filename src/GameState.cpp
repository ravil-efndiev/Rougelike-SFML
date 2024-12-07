#include "Math.hpp"
#include "GameState.hpp"
#include "Components/Sprite.hpp"
#include "Components/Transform.hpp"
#include "Components/SpriteSystems.hpp"
#include "Components/CollisionSystems.hpp"
#include "Gameplay/Player.hpp"
#include "Gameplay/AttackHitbox.hpp"
#include "Gameplay/Enemy.hpp"
#include "Gameplay/Tilemap.hpp"

GameState::GameState(Scene& scene) : mScene(scene) {
    mScene
        .addSystem(spriteTransformSystem)
        .addSystem(spriteAnimationSystem)
        .addSystem(colliderPositionSystem)

        .addSystem(playerMovementSystem)
        .addSystem(playerCombatSystem)
        .addEventSystem(playerEventSystem)
        .addSystem(enemyAISystem)
        .addSystem(attackHitboxSystem);

    initPlayer(mScene);
    for (i32 i = 0; i < 5; i++)
        spawnEnemy(mScene, EnemyType::undeadMelee, {Random::rangef(300.f, 600.f), Random::rangef(10.f, 300.f)});
}

void GameState::update() {
    mScene.update();
}
