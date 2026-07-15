cmake_minimum_required(VERSION 3.20)

project(GameEngineExamples VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/.. ${CMAKE_CURRENT_BINARY_DIR}/GameEngine)

add_executable(GameEngineExample
    main.cpp
)

target_link_libraries(GameEngineExample
    PRIVATE
        GameEngineLib
)