cmake_minimum_required(VERSION 3.19)
project(Kinematics)

set(CMAKE_CXX_STANDARD 14)

add_executable(Kinematics main.cpp Kinematics3D/kinematics3d.cpp Kinematics3D/kinematics3d.h)