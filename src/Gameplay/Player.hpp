#pragma once
#include <Entity.hpp>

enum class MoveDirection {
    up, down, left, right
};

struct Player {
    const float moveSpeed = 200.f;

    MoveDirection direction = MoveDirection::down;
};

void initPlayer(Entity player);

void playerSystem(const std::vector<Entity>& entities);
