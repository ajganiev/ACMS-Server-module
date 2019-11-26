#ifndef ACMS_SERVER_PROTOCOL_APP_H
#define ACMS_SERVER_PROTOCOL_APP_H

/**
 * Message types used in Application protocol
 **/
typedef enum MSG_TYPE {
    T_HANDSHAKE,
    T_PLANE_INFO,
    T_PLANE_ONLINE,
    T_PLANE_OFFLINE,
    T_CHANGE_ROUTE
};


typedef struct AP_MSG {
    //Message header
    int id;
    int command;
    int status;
    int ack_id;
    int recv_id;
    int size;
    //body
    char body[];
};

struct AP_MSG ap_msg_htonl(struct AP_MSG msg);

#endif //ACMS_SERVER_PROTOCOL_APP_H
