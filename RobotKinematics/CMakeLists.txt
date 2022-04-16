cmake_minimum_required(VERSION 3.19)
project(RobotKinematics)

set(CMAKE_CXX_STANDARD 14)

add_executable(RobotKinematics main.cpp RobotKinematics/Joint.h RobotKinematics/Matrix.h RobotKinematics/DHParameters.h RobotKinematics/Calculator.h RobotKinematics/Chain.h RobotKinematics/pos3d.h RobotKinematics/IK.h)