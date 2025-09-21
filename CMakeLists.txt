cmake_minimum_required(VERSION 3.10)

project(CPP_CACHE LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


file(GLOB SOURCES source/*.cpp)

add_executable(my_program ${SOURCES})

target_include_directories(my_program PRIVATE include)
