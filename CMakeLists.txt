cmake_minimum_required(VERSION 3.15)
project(acms_server C)

set(CMAKE_C_STANDARD 99)

add_executable(server server/server.c server/mqueue.h airplane/plane.h protocol/ap_data.h protocol/app_sv_handlers.h)
add_executable(client server/client.c server/mqueue.h server/socket.h server/config.h server/console.h airplane/plane.h protocol/ap_data.h protocol/app_cl_handlers.h protocol/proto-common.h)