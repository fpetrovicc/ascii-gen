# Minimum version of CMake
cmake_minimum_required(VERSION 3.28)

# C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Project name
project(ascii-gen)

# Find Magick++ package
find_package(PkgConfig REQUIRED)
pkg_check_modules(MAGICK REQUIRED Magick++)

# Sources
set (SOURCES
     ./asciigen.cpp
)

# Adding executable
add_executable(ascii-gen ${SOURCES})

# Included directories
include_directories(${MAGICK_INCLUDE_DIRS})

# Linking
target_link_libraries(ascii-gen ${MAGICK_LIBRARIES})
