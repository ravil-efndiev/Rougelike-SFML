cmake_minimum_required(VERSION 3.28)
project(game)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(UNIX)
    set(CMAKE_TOOLCHAIN_FILE ${HOME}/tools/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "")
    if(LINUX)
        set(SFML_DIR ~/tools/vcpkg/installed/x64-linux/share/sfml)
        set(FREETYPE_DIR ${HOME}/tools/vcpkg/installed/x64-linux/share/freetype)
    endif()
endif()

add_executable(game 
    src/main.cpp
    src/Game.cpp
    src/ECS/Scene.cpp
    src/ECS/ComponentStorage.cpp
)

target_include_directories(game PUBLIC
    src/
    src/ECS
)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
target_link_libraries(game PRIVATE sfml-system sfml-network sfml-graphics sfml-window)
