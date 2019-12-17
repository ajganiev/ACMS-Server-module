#ifndef ACMS_SERVER_MESSAGE_H
#define ACMS_SERVER_MESSAGE_H

#define PLANE_ID_SIZE 10
#define MAX_MSG_SIZE 30;

typedef struct {
    char a[PLANE_ID_SIZE];
    char b[PLANE_ID_SIZE];
    char msg[MAX_MSG_SIZE];
};

#endif //ACMS_SERVER_MESSAGE_H
