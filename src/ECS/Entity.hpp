#pragma once
#include "Scene.hpp"

class Entity {
public:
    Entity() = default;
    explicit Entity(EntityId id, Scene* scene) : mId(id), mScene(scene) {}

    EntityId getId() const { return mId; }

    bool operator==(const Entity& other) const { return mId == other.mId; }
    bool operator!=(const Entity& other) const { return mId != other.mId; }

    template <class ComponentT, class ...Args>
    ComponentT* add(Args&&... args) const {
        return mScene->mRegistry.add<ComponentT>(mId, ComponentT(std::forward<Args>(args)...));
    }

    template <class ComponentT>
    ComponentT* get() const {
        return mScene->mRegistry.get<ComponentT>(mId);
    }

    template <class ComponentT>
    bool has() const {
        return mScene->mRegistry.has<ComponentT>(mId);
    }

    template <class ComponentT>
    void remove() const {
        mScene->mRegistry.remove<ComponentT>(mId);
    }

private:
    EntityId mId;
    Scene* mScene;
};
