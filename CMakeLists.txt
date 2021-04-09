cmake_minimum_required(VERSION 3.17)
project(AES)

set(CMAKE_CXX_STANDARD 14)
FILE(COPY simple_input.txt input.txt DESTINATION "${CMAKE_BINARY_DIR}")

add_executable(AES main.cpp Utils.cpp Utils.h RijndaelAlgorithm.h RijndaelAlgorithm.cpp)