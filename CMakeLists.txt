cmake_minimum_required(VERSION 2.8)
project(Poseidon C CXX)

set(CMAKE_C_STANDARD 99)
include_directories(/usr/include/readline)

find_package(Threads REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -lreadline -lpthread")


add_subdirectory(Poseidon)