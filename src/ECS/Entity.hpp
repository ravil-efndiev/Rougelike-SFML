#pragma once
#include "include.hpp"

class Scene;
class Entity {
public:
    explicit Entity(i32 id, Scene* scene) : mId(id), mScene(scene) {}

    i32 getId() const { return mId; }

    bool operator==(const Entity& other) const { return mId == other.mId; }
    bool operator!=(const Entity& other) const { return mId != other.mId; }

    template <class ComponentT, class ...Args>
    ComponentT* add(Args&&... args) {
        return mScene->mStorage.add<ComponentT>(*this, std::forward<Args>(args)...);
    }

    template <class ComponentT>
    ComponentT* get() {
        return mScene->mStorage.get<ComponentT>(*this);
    }

private:
    i32 mId;
    Scene* mScene;
};

struct EntityHasher {
    std::size_t operator()(const Entity& entity) {
        return std::hash<i32>()(entity.getId());
    }
};
