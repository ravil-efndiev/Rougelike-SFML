#pragma once
#include "ComponentStorage.hpp"

class ComponentRegistry {
public:
    ComponentRegistry() = default;
    ~ComponentRegistry() = default;

    template<class ComponentT>
    ComponentT* add(EntityId entityId, const ComponentT& component) {
        return getOrCreateStorage<ComponentT>().add(entityId, component);
    }

    template<class ComponentT>
    ComponentT* get(EntityId entityId) {
        return getOrCreateStorage<ComponentT>().get(entityId);
    }

    template<class ComponentT>
    bool has(EntityId entityId) {
        return getOrCreateStorage<ComponentT>().has(entityId);
    }

    template<class ComponentT>
    void remove(EntityId entityId) {
        getOrCreateStorage<ComponentT>().remove(entityId);
    }

private:
    template <class ComponentT>
    ComponentStorage<ComponentT>& getOrCreateStorage() {
        std::type_index type = typeid(ComponentT);

        if (mAllStorages.find(type) == mAllStorages.end()) {
            mAllStorages.emplace(type, newPtr<ComponentStorage<ComponentT>>());
        }

        return *static_cast<ComponentStorage<ComponentT>*>(mAllStorages.at(type).get());
    }

    std::unordered_map<std::type_index, Ptr<IComponentStorage>> mAllStorages;
};
