//
// Created by sarge on 12.12.2019.
//

#ifndef ACMS_SERVER_APP_SV_HANDLERS_H
#define ACMS_SERVER_APP_SV_HANDLERS_H

#define TEST_LOGIN "ajganiev"
#define TEST_PASS "123"

#include "string.h"
#include "../server/socket.h"

void handle_p_auth(socket_peer *peer, g_msg *msg) {
    g_msg answer;
    p_auth_resp resp;
    p_auth *pck = (p_auth*) msg->payload;
    if(strcmp(pck->login, TEST_LOGIN) == 0 && strcmp(pck->pass, TEST_PASS) == 0) resp.status = 1;
    else resp.status = 0;
    init_pl_size();
    prepare_packet(1, "SERVER_NAME", &answer, &resp);
    log_msg(&answer);
    mq_send(peer, &answer);
}

#endif //ACMS_SERVER_APP_SV_HANDLERS_H
