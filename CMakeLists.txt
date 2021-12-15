cmake_minimum_required(VERSION 3.10)

# Set project name
project(chip8)

# Set source files
set(SOURCE_FILES src/main.cpp src/chip8.cpp src/window.cpp src/tests.cpp)

# Add the executable
add_executable(chip8 ${SOURCE_FILES})

INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED)
INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(chip8 ${SDL2_LIBRARIES})
