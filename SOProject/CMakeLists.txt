cmake_minimum_required(VERSION 3.19)
project(SOProject C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

include_directories(.)

add_executable(SOProject main.c)