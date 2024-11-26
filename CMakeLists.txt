cmake_minimum_required(VERSION 3.15)
project(game)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_TOOLCHAIN_FILE ${HOME}/tools/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "")

add_executable(game 
    src/main.cpp
    src/Game.cpp
)

target_include_directories(game PUBLIC
    src/
    src/ECS
)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
target_link_libraries(game PRIVATE sfml-system sfml-network sfml-graphics sfml-window)
