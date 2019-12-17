#ifndef ACMS_SERVER_AIRPORT_DATA_H
#define ACMS_SERVER_AIRPORT_DATA_H

typedef struct {
    char name[30];
    int delayed;
    int arriving;
    int delayed;
    int in_air;
} airport_data;

#endif //ACMS_SERVER_AIRPORT_DATA_H
