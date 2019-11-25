#ifndef ACMS_SERVER_PROTOCOL_APP_H
#define ACMS_SERVER_PROTOCOL_APP_H

/**
 * Message types used in Application protocol
 **/

//Plane
#define T_PLANE_INFO 0;
#define T_PLANE_ONLINE 1;
#define T_PLANE_OFFLINE 2;
#define T_CHANGE_ROUTE 3;

#define T_HANDSHAKE 5;

typedef struct APP_PROTO_MSG {
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

struct APP_PROTO_MSG encrypt(struct APP_PROTO_MSG);
struct APP_PROTO_MSG decrypt(struct APP_PROTO_MSG);
int verify_protocol(struct APP_PROTO_MSG msg);
char* proto_encode_msg(struct msg);
struct APP_PROTO_MSG proto_decode_msg(char*);
#endif //ACMS_SERVER_PROTOCOL_APP_H
