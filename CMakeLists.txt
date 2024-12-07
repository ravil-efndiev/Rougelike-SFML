cmake_minimum_required(VERSION 3.22)
project(game)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(UNIX)
    set(CMAKE_TOOLCHAIN_FILE ~/tools/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "")
    if(LINUX)
        set(SFML_DIR ~/tools/vcpkg/installed/x64-linux/share/sfml)
        set(FREETYPE_DIR ~/tools/vcpkg/installed/x64-linux/share/freetype)
    endif()
endif()

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB_RECURSE SRC_FILES ${SOURCE_DIR}/*.cpp)

add_executable(game ${SRC_FILES})

target_include_directories(game PUBLIC
    src/
    src/ECS
)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)

target_link_libraries(game PRIVATE sfml-system sfml-network sfml-graphics sfml-window)
target_link_libraries(game PRIVATE ImGui-SFML::ImGui-SFML)
target_link_libraries(game PRIVATE yaml-cpp::yaml-cpp)
