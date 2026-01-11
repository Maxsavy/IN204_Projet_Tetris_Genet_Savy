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

# --- Executable 1: The game ---
add_executable(window src/window_grid.cpp)
target_compile_features(window PRIVATE cxx_std_17)

target_link_libraries(window PRIVATE 
    sfml-graphics
    sfml-audio
    sfml-network
)


