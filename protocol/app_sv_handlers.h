#ifndef ACMS_SERVER_APP_SV_HANDLERS_H
#define ACMS_SERVER_APP_SV_HANDLERS_H

#define TEST_LOGIN "ajganiev"
#define TEST_PASS "123"

#include "string.h"


p_auth_resp handle_p_auth(g_msg *msg) {
    p_auth_resp resp;
    p_auth *pck = (p_auth*) msg->payload;
    if(strcmp(pck->login, TEST_LOGIN) == 0 && strcmp(pck->pass, TEST_PASS) == 0) resp.status = 1;
    else resp.status = 0;
    return resp;
}

#endif //ACMS_SERVER_APP_SV_HANDLERS_H
