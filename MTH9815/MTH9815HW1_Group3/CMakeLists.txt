# Minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project name
project(MyProject)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)

# Add source files
set(SOURCE_FILES
    src/main.cpp
)

# Create the executable
add_executable(my_executable ${SOURCE_FILES})
