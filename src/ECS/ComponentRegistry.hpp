#pragma once
#include "ComponentStorage.hpp"

class ComponentRegistry {
public:
    ComponentRegistry() = default;
    ~ComponentRegistry() = default;

    template<class ComponentT>
    ComponentT* add(EntityId entityId, const ComponentT& component) {
        auto* cmp = getOrCreateStorage<ComponentT>().add(entityId, component);
        R_ASSERT(cmp, "component already exist on an entity")
        return cmp;
    }

    template<class ComponentT>
    ComponentT* get(EntityId entityId) {
        auto* cmp = getOrCreateStorage<ComponentT>().get(entityId);
        R_ASSERT(cmp, "component does not exist")
        return cmp;
    }

    template<class ComponentT>
    bool has(EntityId entityId) {
        return getOrCreateStorage<ComponentT>().has(entityId);
    }

    template<class ComponentT>
    void remove(EntityId entityId) {
        getOrCreateStorage<ComponentT>().remove(entityId);
    }

    void clear(EntityId entityId) {
        for (auto& [_, storage] : mAllStorages) {
            storage->remove(entityId);
        }
    }

private:
    template <class ComponentT>
    ComponentStorage<ComponentT>& getOrCreateStorage() {
        std::type_index type = typeid(ComponentT);

        if (mAllStorages.find(type) == mAllStorages.end()) {
            mAllStorages.emplace(type, newRef<ComponentStorage<ComponentT>>());
        }

        return *static_cast<ComponentStorage<ComponentT>*>(mAllStorages.at(type).get());
    }

    std::unordered_map<std::type_index, Ref<IComponentStorage>> mAllStorages;
};
