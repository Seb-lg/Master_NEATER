cmake_minimum_required(VERSION 3.13)
project(Master_NEATER)

set(CMAKE_CXX_STANDARD 17)
find_package (Threads REQUIRED)

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
        include/Helper.hpp
        src/IOANN.hpp
#        src/ALEWrapper.cpp
#        src/ALEWrapper.hpp
        src/SnakeWrapper.cpp
        src/SnakeWrapper.hpp
        src/Graphical/Graphical.cpp
        src/Graphical/Graphical.hpp src/Specie.cpp src/Specie.hpp src/Genus.cpp src/Genus.hpp)

#target_link_libraries (Master_NEATER Threads::Threads)
target_link_libraries(Master_NEATER ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(Master_NEATER /usr/lib/x86_64-linux-gnu/libsfml-graphics.so)
target_link_libraries(Master_NEATER /usr/lib/x86_64-linux-gnu/libsfml-window.so)
target_link_libraries(Master_NEATER /usr/lib/x86_64-linux-gnu/libsfml-system.so)