cmake_minimum_required(VERSION 3.10)
project(GameOfLife)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(game_of_life
    src/main.cpp
    src/GameOfLife.cpp
    src/CLI.cpp
)
