cmake_minimum_required(VERSION 3.19)
project(SimpleNeural)

set(CMAKE_CXX_STANDARD 14)

add_executable(SimpleNeural main.cpp networktools.h neuralnet/network.cpp neuralnet/networklayer.cpp)