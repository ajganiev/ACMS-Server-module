#ifndef ACMS_SERVER_2JSON_H
#define ACMS_SERVER_2JSON_H


int p_auth_resp_print(struct json_out *out, va_list *ap) {
    p_auth_resp *p = va_arg(*ap,  p_auth_resp *);
    return  json_printf(out,
                        "{status: %d}",
                        p->status);
}

void p_auth_resp2json(char *out, size_t size, g_msg *data) {
    struct json_out outBuf = JSON_OUT_BUF(out, size);
    json_printf(&outBuf,
                "{command: %d, id:%d,  sender: %Q, payload: %M}",
                data->command,
                data->id,
                data->sender,
                p_auth_resp_print,
                data->payload);
}

#endif //ACMS_SERVER_2JSON_H
