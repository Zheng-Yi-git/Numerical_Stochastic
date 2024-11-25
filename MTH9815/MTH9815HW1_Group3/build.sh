#!/bin/bash

# Create and navigate to the build directory
mkdir -p build
cd build

# Run CMake to generate Makefile
cmake ..

# Compile the project
make
