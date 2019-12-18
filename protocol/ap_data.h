#ifndef ACMS_SERVER_AP_DATA_H
#define ACMS_SERVER_AP_DATA_H

#include "../server/msg.h"

/**
 * Only data structures
 */

typedef enum {
    P_AUTH = 0,
    P_AUTH_RESP = 1
} APP_CMD;

typedef struct {
    char name[10];
    char login[10];
    char pass[10];
} p_auth;

void generate_p_auth_JSON(char *out, size_t size, p_auth *data) {
    struct json_out outBuf = JSON_OUT_BUF(out, size);
    json_printf(&outBuf,
                "{name: %Q, login:%Q,  pass: %Q}",
                data->name,
                data->login,
                data->pass);
}

typedef struct {
    int status;
} p_auth_resp;


#endif //ACMS_SERVER_AP_DATA_H
