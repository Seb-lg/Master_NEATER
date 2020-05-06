cmake_minimum_required(VERSION 3.13)
project(Master_NEATER)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(Master_NEATER
        Main.cpp
        src/Node.cpp
        src/Node.hpp
        src/Connection.cpp
        src/Connection.hpp
        src/NeuralNetwork.cpp
        src/NeuralNetwork.hpp
        src/Zone.cpp
        src/Zone.hpp
        include/Helper.hpp)
