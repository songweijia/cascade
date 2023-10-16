cmake_minimum_required(VERSION 3.15)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
project(triebench CXX)

# Version
set(triebench_VERSION 1.0rc0)
set(triebench_build_VERSION 1.0rc0)

# C++ STANDARD
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_FLAGS "-Ofast -march=native")

find_package(cascade REQUIRED)

add_executable(triebench main.cpp)
target_link_libraries(triebench cascade)
