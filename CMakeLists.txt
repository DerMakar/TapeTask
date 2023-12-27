cmake_minimum_required(VERSION 3.10)

project(yardro_tape_sorter)

set(CMAKE_CXX_STANDARD 17)

add_executable(tape_sort
        src/main.cpp
        src/tape.cpp src/tape.h
        src/tapeinter.h
        src/tapesort.cpp src/tapesort.h
        src/exceptions.cpp src/exceptions.h)