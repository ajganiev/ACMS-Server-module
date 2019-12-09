cmake_minimum_required(VERSION 3.15)
project(acms-server C)

set(CMAKE_C_STANDARD 99)

add_executable(server server/server.c server/mqueue.h)
add_executable(client server/client.c server/mqueue.h server/socket.h server/message.h server/config.h)