cmake_minimum_required(VERSION 3.14)
project(i3x_status)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

include_directories(include)
add_subdirectory(src)
add_subdirectory(test)
