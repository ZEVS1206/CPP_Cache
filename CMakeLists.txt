cmake_minimum_required(VERSION 3.10)

project(CPP_CACHE LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


file(GLOB SOURCES source/*.cpp)
#Cache_2Q
add_executable(cache_2q source/2q_main.cpp)

target_include_directories(cache_2q PRIVATE include)

#Ideal_cache
add_executable(ideal_cache source/ideal_cache.cpp)

target_include_directories(ideal_cache PRIVATE include)