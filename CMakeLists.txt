cmake_minimum_required(VERSION 3.28)
project(ProjetTetrisSG LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# --- Dependencies ---

include(FetchContent)

# 1. Fetch SFML 2.6.1
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# 2. Fetch GameMenu
FetchContent_Declare(gamemenu
    GIT_REPOSITORY https://github.com/ParadoxZero/GameMenu-cpp.git
    GIT_TAG master
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(gamemenu)

# --- Executable 1: The Game (Tetris) ---
add_executable(tetris src/main.cpp)
target_compile_features(tetris PRIVATE cxx_std_17)

# FIX: Use lowercase target names (sfml-graphics) for SFML 2 built from source
target_link_libraries(tetris PRIVATE 
    sfml-graphics
    sfml-audio
    sfml-network
    gamemenu 
)

# --- Executable 2: The Test ---

add_executable(test src/test.cpp)
target_compile_features(test PRIVATE cxx_std_17)

# FIX: Use lowercase target names here too
target_link_libraries(test PRIVATE 
    sfml-graphics
    sfml-audio
    sfml-network
    gamemenu
)