#include "BloodEffect.hpp"
#include <Components/Sprite.hpp>
#include <Components/Animator.hpp>

Entity initBloodEffect(Scene& scene) {
    Entity blood = scene.newEntity();

    auto tex = newRef<sf::Texture>();
    tex->loadFromFile("../assets/textures/blood.png");
    blood.add<Sprite>(tex);

    auto* animator = blood.add<Animator>();
    animator->addAnimation("splash", {6, 0, 0, 3, 48});

    blood.add<BloodEffect>();
    blood.get<Transform>()->scale = {3.f, 3.f};

    return blood;
}

void bloodEffectSystem(const std::vector<Entity>& entities) {
    for (const auto& entity : entities) {
        if (!entity.has<BloodEffect>()) continue;
        auto* blood = entity.get<BloodEffect>();

        if (blood->active) {
            auto* animator = entity.get<Animator>();
            animator->play("splash");
            if (animator->currentAnimation->isFinished()) {
                blood->active = false;
            }
        }    
    }
}
