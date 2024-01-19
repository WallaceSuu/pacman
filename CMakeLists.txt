cmake_minimum_required(VERSION 3.23)
project(pacman)

set(CMAKE_C_STANDARD 99)

add_executable(pacman.cpp colours.cpp
        pacman.cpp)
