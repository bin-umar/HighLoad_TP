cmake_minimum_required(VERSION 3.14)
project(HighLoad_TP)

set(CMAKE_CXX_STANDARD 14)
add_executable(HighLoad_TP main.cpp)
target_link_libraries(HighLoad_TP uv)