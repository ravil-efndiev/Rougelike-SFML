#pragma once
#include "Scene.hpp"

class Entity {
public:
    explicit Entity(EntityId id, Scene& scene) : mId(id), mScene(scene) {}

    EntityId getId() const { return mId; }

    bool operator==(const Entity& other) const { return mId == other.mId; }
    bool operator!=(const Entity& other) const { return mId != other.mId; }

    template <class ComponentT>
    ComponentT* add(const ComponentT& component) {
        return mScene.mRegistry.add<ComponentT>(mId, component);
    }

    template <class ComponentT>
    ComponentT* get() {
        return mScene.mRegistry.get<ComponentT>(mId);
    }

    template <class ComponentT>
    bool has() {
        return mScene.mRegistry.has<ComponentT>(mId);
    }

    template <class ComponentT>
    void remove() {
        mScene.mRegistry.remove<ComponentT>(mId);
    }

private:
    EntityId mId;
    Scene& mScene;
};
