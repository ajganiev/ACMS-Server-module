//
// Created by sarge on 12.12.2019.
//

#ifndef ACMS_SERVER_APP_CL_HANDLERS_H
#define ACMS_SERVER_APP_CL_HANDLERS_H


int handle_p_auth_resp(socket_peer *peer, g_msg *msg) {
    p_auth_resp *pck = (p_auth_resp*) msg->payload;
    if(pck->status == 0) printf("[ACMS] Not authorized!\n");
    else printf("[ACMS] Authorized!\n");
    return 0;
}

#endif //ACMS_SERVER_APP_CL_HANDLERS_H
