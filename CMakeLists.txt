# cmake_minimum_required(VERSION <specify CMake version here>)
project(acms_server C)

set(CMAKE_C_STANDARD 99)

add_executable(acms_server main.c protocols/protocol-app.h protocols/protocol-agc.h structures/plane.h)