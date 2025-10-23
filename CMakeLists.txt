# Top-level CMake configuration for CheatEngine educational tool.
cmake_minimum_required(VERSION 3.20)

project(CheatEngine
    VERSION 0.1.0
    DESCRIPTION "Educational macOS memory introspection tool"
    LANGUAGES CXX)

if(NOT CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    message(FATAL_ERROR "CheatEngine currently targets macOS (Darwin) platforms only.")
endif()

# Default to Debug builds when no build type is specified.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable verbose warnings and debug symbols for educational clarity.
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")

add_executable(cheatengine
    src/main.cpp
)

target_include_directories(cheatengine
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(cheatengine PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wimplicit-fallthrough
    )
endif()

# Ensure we can access Mach APIs and low-level Darwin interfaces.
target_compile_definitions(cheatengine PRIVATE
    _DARWIN_C_SOURCE
)

enable_testing()
