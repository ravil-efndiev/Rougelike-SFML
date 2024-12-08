#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <array>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <memory>
#include <any>
#include <functional>
#include <typeindex>
#include <cmath>
#include <cassert>
#include <random>

#include <SFML/Graphics.hpp>

#define R_DEV true

#define R_ASSERT(condition, errMessage) if (!(condition)) { \
    std::cerr << errMessage << "\n"; \
    std::abort(); \
}

typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef float  f32;
typedef double f64;

typedef u32 EntityId;

template <class T>
using Ptr = std::unique_ptr<T>;

template <class T>
using Ref = std::shared_ptr<T>;

template<class T, class ...Args>
Ptr<T> newPtr(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<class T, class ...Args>
Ref<T> newRef(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}
