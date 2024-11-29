#pragma once
#include <include.hpp>

class Entity;
using System = std::function<void(const std::vector<Entity>&)>;
using EventSystem = std::function<void(const sf::Event&, const std::vector<Entity>&)>;
