#include "Player.hpp"
#include "Health.hpp"
#include "AttackHitbox.hpp"
#include <Components/Transform.hpp>
#include <Components/Sprite.hpp>
#include <Components/Collider.hpp>
#include <Components/Tag.hpp>
#include <Components/Animator.hpp>
#include <Time.hpp>
#include <Game.hpp>

struct DirectionData {
    std::string name;
    MoveDirection direction;
    sf::Vector2f movementVec;
};

static const std::array<DirectionData, 4> directions {{
    {"left",  MoveDirection::left,  { -1.f, 0.f }},
    {"right", MoveDirection::right, { 1.f, 0.f }},
    {"down",  MoveDirection::down,  { 0.f, 1.f }},
    {"up",    MoveDirection::up,    { 0.f, -1.f }},
}};

void initPlayer(Scene& scene) {
    Entity player = scene.newEntity("player");
    Entity atkHitbox = scene.newEntity();

    player.add<Player>();
    atkHitbox.add<AttackHitbox>(40.f, 70.f);
    atkHitbox.add<Collider>(sf::Vector2f(20.f, 20.f))->debugRender = false;

    Ref<sf::Texture> tex = newRef<sf::Texture>();
    tex->loadFromFile("../assets/textures/player_sheet.png");
    player.add<Sprite>(tex);

    auto* tf = player.get<Transform>();
    tf->scale = { 3.f, 3.f };

    player.add<Collider>(sf::Vector2f(30, 60))->debugRender = false;
    player.add<Health>([](i32 newHealth) {
        if (newHealth <= 0) {
            std::cout << "dead\n";
        }
    });

    auto* animator = player.add<Animator>();
    animator->addAnimation("idle_down",  { 30, 0, 0, 3, 48 });
    animator->addAnimation("idle_right", { 30, 0, 2, 3, 48 });
    animator->addAnimation("idle_up",    { 30, 0, 4, 3, 48 });
    animator->addAnimation("idle_left",  { 30, 0, 6, 3, 48 });

    animator->addAnimation("move_down",  { 10, 0, 1, 5, 48 });
    animator->addAnimation("move_right", { 10, 0, 3, 5, 48 });
    animator->addAnimation("move_up",    { 10, 0, 5, 5, 48 });
    animator->addAnimation("move_left",  { 10, 0, 7, 5, 48 });

    animator->addAnimation("attack_right1", { 10, 0, 8, 3, 48 }, "attacks");
    animator->addAnimation("attack_right2", { 10, 4, 8, 7, 48 }, "attacks");
    animator->addAnimation("attack_left1",  { 10, 0, 9, 3, 48 }, "attacks");
    animator->addAnimation("attack_left2",  { 10, 4, 9, 7, 48 }, "attacks");

    animator->addAnimation("attack_down1", { 10, 0, 10, 3, 48 }, "attacks");
    animator->addAnimation("attack_down2", { 10, 4, 10, 7, 48 }, "attacks");
    animator->addAnimation("attack_up1",   { 10, 0, 11, 3, 48 }, "attacks");
    animator->addAnimation("attack_up2",   { 10, 4, 11, 7, 48 }, "attacks");

    animator->setAnimationsTrigger(animator->animCategories.at("attacks"), [player, atkHitbox](i32 frame) {
        auto* playerCmp = player.get<Player>();
        auto* playerTf = player.get<Transform>();
        auto* playerSprite = player.get<Sprite>();
        auto* hitbox = atkHitbox.get<AttackHitbox>();
        auto* atkHitboxCollider = atkHitbox.get<Collider>();
        auto* atkHitboxTf = atkHitbox.get<Transform>();

        if (frame == 1) {
            spawnAttackHitbox(
                playerCmp->bufferedAttack ? playerCmp->bufferedDirection : playerCmp->direction,
                hitbox, atkHitboxCollider, atkHitboxTf, playerTf, playerSprite
            );
        }
        else if (frame == 3) {
            turnOffHitbox(atkHitboxCollider);
        }
    });
}

void move(MoveDirection dir, Player* player, Animator* animator, sf::Vector2f& movement) {
    if (player->attack) return;
    player->direction = dir;
    for (const auto& [name, direction, movementVec] : directions) {
        if (direction == dir) {
            movement += movementVec;
            animator->play("move_" + name);
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
            animator->play("idle_" + directionNames.at(player->direction));
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
            animator->play("attack_" + directionNames.at(player->direction) + "1");
            if (animator->currentAnimation->isFinished()) {
                player->attack = false;
                player->combo = true;
            }
        }

        if (player->combo) {
            player->comboTimer += 100.f * Time::dt();
            if (player->attack || player->bufferedAttack) {
                if (player->bufferedAttack) {
                    animator->play("attack_" + directionNames.at(player->bufferedDirection) + "2");
                }
                else {
                    animator->play("attack_" + directionNames.at(player->direction) + "2");
                }

                if (animator->currentAnimation->isFinished()) {
                    if (player->bufferedAttack)
                        player->direction = player->bufferedDirection;
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

MoveDirection playerMouseDir(const sf::Vector2f& playerPos, const sf::Vector2f& mousePos) {
    f32 dx = mousePos.x - playerPos.x;
    f32 dy = mousePos.y - playerPos.y;

    if (abs(dx) > abs(dy)) {
        return dx > 0 ? MoveDirection::right : MoveDirection::left;
    }
    else {
        return dy > 0 ? MoveDirection::down : MoveDirection::up; // inverted cause sfml
    }
}

void playerEventSystem(const sf::Event& event, const std::vector<Entity>& entities) {
    if (event.type != sf::Event::MouseButtonPressed) return;

    Game* game = Game::getInstance();
    sf::Vector2i mousePixel = sf::Mouse::getPosition(game->window());
    sf::Vector2f mousePos = game->window().mapPixelToCoords(mousePixel);

    for (const auto& entity : entities) {
        if (!entity.has<Player>() 
            || !entity.has<Transform>() 
            || !entity.has<Sprite>()) continue;

        auto* player = entity.get<Player>();
        auto* tf = entity.get<Transform>();
        auto* sprite = entity.get<Sprite>();
        MoveDirection mouseDir = playerMouseDir(sprite->centerPosition(tf->position), mousePos);

        if (player->attack) {
            player->bufferedAttack = true;
            player->bufferedDirection = mouseDir;
        }
        else {
            player->attack = true;
            player->direction = mouseDir;
        }
    }
}

void Player::resetAttack() {
    attack = false;
    bufferedAttack = false;
    combo = false;
    comboTimer = 0.f;
}
