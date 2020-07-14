cmake_minimum_required(VERSION 3.13)
project(Master_NEATER)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_STANDARD 17)
find_package (Threads REQUIRED)

include_directories(.)

add_executable(Master_NEATER
        Main.cpp
        src/NEATER/NEAT/Node.cpp
        src/NEATER/NEAT/Node.hpp
        src/NEATER/NEAT/Connection.cpp
        src/NEATER/NEAT/Connection.hpp
        src/NEATER/NEAT/NeuralNetwork.cpp
        src/NEATER/NEAT/NeuralNetwork.hpp
        src/NEATER/Zone.cpp
        src/NEATER/Zone.hpp
        include/Helper.hpp
        src/NEATER/NEAT/IOANN.hpp
#        src/ALEWrapper.cpp
#        src/ALEWrapper.hpp
        src/SnakeWrapper.cpp
        src/SnakeWrapper.hpp
        src/Graphical/Graphical.cpp
        src/Graphical/Graphical.hpp src/NEATER/NEAT/Specie.cpp src/NEATER/NEAT/Specie.hpp src/NEATER/NEAT/Genus.cpp src/NEATER/NEAT/Genus.hpp)

#target_link_libraries (Master_NEATER Threads::Threads)
target_link_libraries(Master_NEATER ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(Master_NEATER /usr/lib/x86_64-linux-gnu/libsfml-graphics.so)
target_link_libraries(Master_NEATER /usr/lib/x86_64-linux-gnu/libsfml-window.so)
target_link_libraries(Master_NEATER /usr/lib/x86_64-linux-gnu/libsfml-system.so)