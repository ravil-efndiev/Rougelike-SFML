#pragma once
#include "State.hpp"
#include "ECS/Scene.hpp"

class GameState : public IState {
public:
    GameState(Scene& scene);
    ~GameState() = default;

    void update() override;
    void renderUI() override {}

private:
    Scene& mScene;
};
