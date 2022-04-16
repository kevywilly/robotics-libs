cmake_minimum_required(VERSION 3.19)
project(SimpleNN)

set(CMAKE_CXX_STANDARD 14)

add_executable(SimpleNN main.cpp SimpleNN/nn.h inputs.h)