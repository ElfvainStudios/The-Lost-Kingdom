cmake_minimum_required(VERSION 3.16)
project(TheLostKingdom VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS "Game/*.cpp")
add_executable(TheLostKingdom ${GAME_SOURCES})
target_include_directories(TheLostKingdom PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Game")

if(MSVC)
    target_compile_options(TheLostKingdom PRIVATE /W4)
else()
    target_compile_options(TheLostKingdom PRIVATE -Wall -Wextra -Wpedantic)
endif()
