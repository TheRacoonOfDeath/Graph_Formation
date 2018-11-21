cmake_minimum_required(VERSION 3.12)
project(moore)

set(CMAKE_CXX_STANDARD 17)

find_package (Eigen3 3.3 REQUIRED NO_MODULE)

add_executable(moore main.cpp response.h )
target_link_libraries (moore Eigen3::Eigen)