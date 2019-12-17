cmake_minimum_required(VERSION 3.15)

project(acms_server C)

set(CMAKE_C_STANDARD 99)



add_executable(server server/server.c server/msg.h airplane/plane.h protocol/ap_data.h protocol/app_sv_handlers.h)
add_executable(client server/client.c server/msg.h server/socket.h server/config.h server/console.h airplane/plane.h protocol/ap_data.h protocol/app_cl_handlers.h protocol/proto-server.h)
add_executable(mq-client client-module/mq-client.c client-module/msg.h client-module/socket.h client-module/config.h client-module/console.h protocol/ap_data.h protocol/app_cl_handlers.h protocol/proto-client.h)
add_executable(mq-recv client-module/reciever.c protocol/ap_data.h)


target_link_libraries(mq-client rt)
target_link_libraries(mq-recv rt)