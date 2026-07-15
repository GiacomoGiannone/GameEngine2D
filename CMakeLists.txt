cmake_minimum_required(VERSION 3.20)

project(GameEngine VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE ENGINE_SOURCES
    CONFIGURE_DEPENDS
    engine/*.cpp
)

include(FetchContent)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
)

FetchContent_MakeAvailable(SFML)

add_library(GameEngineLib STATIC
    ${ENGINE_SOURCES}
)

target_include_directories(GameEngineLib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/engine

    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/engine
)

target_link_libraries(GameEngineLib
    PUBLIC
        SFML::Graphics
        SFML::Window
        SFML::System
        SFML::Audio
        SFML::Network
)