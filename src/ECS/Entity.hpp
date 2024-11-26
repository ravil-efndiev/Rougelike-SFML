#pragma once
#include <include.hpp>

class Entity {
public:
    explicit Entity(i32 id) : mId(id) {}

    i32 getId() const { return mId; }

    bool operator==(const Entity& other) const { return mId == other.mId; }
    bool operator!=(const Entity& other) const { return mId != other.mId; }

private:
    i32 mId;
};
