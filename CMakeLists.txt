cmake_minimum_required(VERSION 2.8)
project(Oceans C Java)

set(CMAKE_C_STANDARD 99)
include_directories(/usr/include/readline)
find_package(Threads REQUIRED)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -lreadline -lpthread -D_PROCESS_ -D_GNU_SOURCE")

find_package(Java REQUIRED)
include(UseJava)

set(POSEIDON_DIR Poseidon)

add_subdirectory(Poseidon/src)
add_subdirectory(Poseidon/tst)
#add_subdirectory(Aquarium/src)

enable_testing()
