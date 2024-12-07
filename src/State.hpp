#pragma once
#include <include.hpp>

class IState {
public:
    virtual ~IState() = default;
    virtual void update() = 0;
    virtual void renderUI() = 0;
    virtual void onEvent(const sf::Event& event) {};
};
