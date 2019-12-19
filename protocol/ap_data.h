#ifndef ACMS_SERVER_AP_DATA_H
#define ACMS_SERVER_AP_DATA_H

#include "../server/msg.h"

/**
 * Only data structures
 */

typedef enum {
    P_AUTH = 0,
    P_AUTH_RESP = 1,
    P_TEST = 2,
    P_AIRPLANE_BROADCAST = 36
} APP_CMD;

typedef struct {
    char name[10];
    char login[10];
    char pass[10];
} p_auth;


typedef struct {
    char uuid[30];
    double velocity[3];
    double position[3];
    double fuel;
    char path_id[20];
} p_plane_data;


typedef struct {
    int status;
} p_auth_resp;

typedef struct {
    int test;
} test_pck;


#endif //ACMS_SERVER_AP_DATA_H


// TEST DATA

typedef struct {
    p_plane_data planes[3];
    int size;
} p_planes;

p_planes testData = {
        {
                {
                        "ICE-01",
                        {0.7359310117618296, 0.6770565308208832, 0},
                        {20, 10, 6000},
                        50,

                },
                {
                        "CSE-01",
                        {0.3757051087392182, 0.9267392682234049, 0},
                        {45, 33, 1000},
                        98,

                },
                {
                        "CSE-02",
                        {0.07441992482162527, -0.9972269926097787, 0},
                        {43.21392180428099, 56.93344782263469, 6000},
                        29

                },

        },
        3
};
