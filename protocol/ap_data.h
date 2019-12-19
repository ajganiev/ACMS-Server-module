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
    P_ROUTE = 3
} APP_CMD;

typedef struct {
    char name[10];
    char login[10];
    char pass[10];
} p_auth;

typedef struct {
    int status;
} p_auth_resp;

typedef struct {
    int test;
} test_pck;

typedef struct {
    char uuid[10];
    char color[7];
    double path[50];
} _route;

typedef struct {
    _route routes[3];
} route;


route routes = {
        {
    {
            "ICE-01",
            "#c00012",
            {0, 0, 20, 10, 45, 33, 60, 70, 100, 100},
    }, 
    {
           "ICE-02",
           "#00ae2a",
           {0, 100, 10, 80, 45, 33, 80, 10, 100, 0},
   },
   {
          "ICE-03",
          "#0019ae",
          {50, 0, 45, 33, 40, 100},
          }
    }
};
route _routes[3] = {};


#endif //ACMS_SERVER_AP_DATA_H
