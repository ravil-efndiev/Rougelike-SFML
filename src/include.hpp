#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <memory>

#include <SFML/Graphics.hpp>

typedef signed short i16;
typedef signed int   i32;
typedef signed long  i64;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef unsigned long  u64;
typedef float  f32;
typedef double f64;

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
