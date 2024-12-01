#include "Player.hpp"
#include <Components/Transform.hpp>
#include <Components/Sprite.hpp>
#include <Components/Collider.hpp>
#include <Components/Tag.hpp>
#include <Components/Animator.hpp>
#include <Time.hpp>
#include <Game.hpp>

struct DirectionData {
    MoveDirection direction;
    sf::Vector2f movementVec;
};

static const std::unordered_map<std::string, DirectionData> directions = {
    {"left",  {MoveDirection::left,  { -1.f, 0.f }}},
    {"right", {MoveDirection::right, { 1.f, 0.f }}},
    {"down",  {MoveDirection::down,  { 0.f, 1.f }}},
    {"up",    {MoveDirection::up,    { 0.f, -1.f }}},
};

static const std::unordered_map<MoveDirection, std::string> directionNames = {
    {MoveDirection::left, "left"},
    {MoveDirection::right, "right"},
    {MoveDirection::down, "down"},
    {MoveDirection::up, "up"},
};

void normalize(sf::Vector2f& vec) {
    f32 magnitude = sqrt(pow(vec.x, 2) + pow(vec.y, 2));
    if (magnitude != 0) vec /= magnitude;
}

void initPlayer(Scene& scene) {
    Entity player = scene.newEntity("player");
    Entity atkHitbox = scene.newEntity();

    player.add<Player>();
    atkHitbox.add<PlayerAttackHitbox>();
    atkHitbox.add<Collider>(sf::Vector2f(20.f, 20.f))->debugRender = true;

    Ref<sf::Texture> tex = newRef<sf::Texture>();
    tex->loadFromFile("../assets/textures/player_sheet.png");
    player.add<Sprite>(tex);

    auto* tf = player.get<Transform>();
    tf->scale = { 3.f, 3.f };

    player.add<Collider>(sf::Vector2f(30, 60))->debugRender = true;

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
        auto* atkHitboxCollider = atkHitbox.get<Collider>();

        if (frame == 1) {
            // TODO: turn hitbox on and modify based on direction
        }
        else if (frame == 3) {
            // TODO: turn hitbox off
        }
    });
}

void move(MoveDirection dir, Player* player, Animator* animator, sf::Vector2f& movement) {
    if (player->attack) return;
    player->direction = dir;
    for (const auto& [directionName, directionData] : directions) {
        if (directionData.direction == dir) {
            movement += directionData.movementVec;
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
            for (const auto& [direction, directionName] : directionNames) {
                if (player->direction == direction) {
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

        for (const auto& [direction, directionName] : directionNames) {
            if (player->direction != direction)
                continue;

            if (player->attack && !player->combo) {
                animator->play("attack_" + directionName + "1");
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
                        animator->play("attack_" + directionName + "2");
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

    for (const auto& entity : entities) {
        if (!entity.has<Player>() 
            || !entity.has<Transform>() 
            || !entity.has<Sprite>()) continue;

        auto* player = entity.get<Player>();
        auto* tf = entity.get<Transform>();
        auto* sprite = entity.get<Sprite>();

        Game* game = Game::getInstance();
        sf::Vector2i mousePixel = sf::Mouse::getPosition(game->window());
        sf::Vector2f mousePos = game->window().mapPixelToCoords(mousePixel);
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

void playerAttackHiboxSystem(const std::vector<Entity>& entities) {
    for (const auto& entity : entities) {
        if (!entity.has<PlayerAttackHitbox>() || !entity.has<Transform>()) continue;

        auto* tf = entity.get<Transform>();

        Entity player = *std::find_if(entities.begin(), entities.end(), [](const Entity& entity) {
            return entity.get<Tag>()->name == "player";
        });

        tf->position = player.get<Transform>()->position;
    }
}
