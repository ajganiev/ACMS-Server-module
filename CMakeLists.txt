cmake_minimum_required(VERSION 3.5.1)

project(acms_server C)

set(CMAKE_C_STANDARD 99)



add_executable(server server/server.c server/msg.h airplane/plane.h protocol/ap_data.h protocol/app_sv_handlers.h structures/plane-data.h)
add_executable(client server/client.c server/msg.h server/socket.h server/config.h server/console.h airplane/plane.h protocol/ap_data.h protocol/app_cl_handlers.h protocol/proto-server.h structures/plane-data.h protocol/2json.h)
add_executable(mq-client client-module/mq-client.c client-module/msg.h client-module/socket.h client-module/config.h client-module/console.h protocol/ap_data.h protocol/app_cl_handlers.h protocol/proto-client.h structures/plane-data.h frozen.c frozen.h)
add_executable(nodejs-recv client-module/reciever.c protocol/ap_data.h structures/plane-data.h)
add_executable(nodejs-send client-module/sender.c client-module/config.h)
add_executable(mq-client-2 client-module/mq-client.c client-module/msg.h client-module/socket.h client-module/config.h client-module/console.h protocol/ap_data.h protocol/app_cl_handlers.h protocol/proto-client.h structures/plane-data.h frozen.c frozen.h)


target_link_libraries(mq-client rt pthread)
target_link_libraries(mq-client-2 rt pthread)
target_link_libraries(server rt pthread)
target_link_libraries(nodejs-recv rt)
target_link_libraries(nodejs-send rt)