#pragma once
#include <ECS/Entity.hpp>
#include "Sprite.hpp"
#include "Animator.hpp"
#include "Transform.hpp"

void spriteTransformSystem(const std::vector<Entity>& entities);

void spriteAnimationSystem(const std::vector<Entity>& entities);
