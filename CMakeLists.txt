cmake_minimum_required(VERSION 3.5)

project(semu)
set(CMAKE_CXX_STANDARD 17)
# todo
option(BUILD_BACKEND "Build backend server." OFF)

include_directories(src)
add_subdirectory(src)
add_subdirectory(app)
