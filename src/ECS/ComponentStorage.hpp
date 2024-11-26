#pragma once
#include "Entity.hpp"

class ComponentStorage
{
public:
    ComponentStorage() = default;
    ~ComponentStorage() = default;

    template <class ComponentT, class ...Args>
    ComponentT* add(Entity entity, Args&&... args) {
        std::vector<std::any> components = mRegistry[entity];

        if (has<ComponentT&>(entity)) {
            return nullptr;
        }

        return &components.push_back(
            std::any_cast<ComponentT&>(
                ComponentT(std::forward<Args>(args)...)
            )
        );

    }

    template <class ComponentT>
    ComponentT* get(Entity entity) {
        std::vector<std::any> components = mRegistry.at(entity);
        for (const auto& component : components) {
            
        }
    }

    template <class ComponentT>
    bool has(Entity entity) {
        std::vector<std::any> components = mRegistry.at(entity);
        for (const auto& component : components) {
            try {
                std::any_cast<ComponentT&>(component);
                return true;
            }
            catch (const std::bad_any_cast&) {
                continue;
            }
            catch (const std::out_of_range&) {
                return false;
                // TODO: throw an error
            }
        }
        return false;
    }

private:
    std::unordered_map<Entity, std::vector<std::any>, EntityHasher> mRegistry;

};
