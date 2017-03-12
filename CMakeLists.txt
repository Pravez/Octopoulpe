cmake_minimum_required(VERSION 3.6)
project(Poseidon)

set(CMAKE_C_STANDARD 99)
include_directories(/usr/include/readline)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -lreadline -lpthread")


add_subdirectory(Poseidon)