cmake_minimum_required(VERSION 3.12)
project(Othello_MC)

set(CMAKE_CXX_STANDARD 14)

include_directories(${INC})

include_directories(.)

add_executable(Othello_MC
        main.cpp
        MCNode.cpp
        MCNode.h
        board.cpp
        board.h constant.h)
