cmake_minimum_required(VERSION 3.29)  # Specify the minimum CMake version required

# Set the project name and version
project(MyProject)
#add_library(MyProject SHARED main.cpp Frame.cpp structure.cpp)

add_executable(MyProject main.cpp Frame.cpp structure.cpp)

