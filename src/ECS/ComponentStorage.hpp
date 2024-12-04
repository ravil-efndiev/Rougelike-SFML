#pragma once
#include <include.hpp>

class IComponentStorage {
public:
    virtual ~IComponentStorage() = default;

    virtual void remove(EntityId entitId) = 0;
};

template <class ComponentT>
class ComponentStorage : public IComponentStorage
{
public:
    ComponentStorage() = default;
    ~ComponentStorage() = default;

    ComponentT* add(EntityId entityId, const ComponentT& component) {
        if (has(entityId)) 
            return nullptr;

        mComponents.push_back(component);
        mEntities.push_back(entityId);
        size_t index = mComponents.size() - 1;
        mEntityIdToIndex[entityId] = index;
        return &mComponents[index];
    }

    ComponentT* get(EntityId entityId) {
        if (!has(entityId)) 
            return nullptr;

        return &mComponents[mEntityIdToIndex.at(entityId)];
    }

    bool has(EntityId entityId) const {
        return mEntityIdToIndex.find(entityId) != mEntityIdToIndex.end();
    }

    void remove(EntityId entityId) override {
        if (!has(entityId)) return;
        if (mComponents.empty()) return;

        size_t currentIndex = mEntityIdToIndex[entityId];
        size_t lastIndex = mComponents.size() - 1;

        if (currentIndex != lastIndex) {
            std::swap(mComponents[currentIndex], mComponents[lastIndex]);
            std::swap(mEntities[currentIndex], mEntities[lastIndex]);

            mEntityIdToIndex[mEntities[currentIndex]] = currentIndex;
        }

        mEntityIdToIndex.erase(entityId);
        mComponents.pop_back();
        mEntities.pop_back();
    }

private:
    std::vector<ComponentT> mComponents;
    std::vector<EntityId> mEntities;
    std::unordered_map<EntityId, size_t> mEntityIdToIndex;
};
