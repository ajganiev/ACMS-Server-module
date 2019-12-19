#ifndef ACMS_SERVER_2JSON_H
#define ACMS_SERVER_2JSON_H


int p_auth_resp_print(struct json_out *out, va_list *ap) {
    p_auth_resp *p = va_arg(*ap, p_auth_resp *);
    return json_printf(out,
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

int p_planes2json(struct json_out *out, p_plane_data *data) {
    return json_printf(out,
                       "{uuid: %Q, velocity: [%f, %f, %f],  position: [%f, %f, %f], fuel: %f, path_id: %Q}",
                       data->uuid,
                       data->velocity[0],
                       data->velocity[1],
                       data->velocity[2],
                       data->position[0],
                       data->position[1],
                       data->position[2],
                       data->fuel,
                       data->path_id);
}


int p_planes_resp_print(struct json_out *out, va_list *ap) {
    int len = 0, i = 0;
    len += json_printf(out, "[", 1);
    p_planes *p = va_arg(*ap, p_planes *);
    for (; i < p->size; i++) {
        len += p_planes2json(out, &p->planes[i]);
        if (i + 1 < p->size) {
            len += json_printf(out, ",", 1);
        }
    }
    len += json_printf(out, "]", 1);
    return len;
}

void p_planes_resp2json(char *out, size_t size, g_msg *data) {
    struct json_out outBuf = JSON_OUT_BUF(out, size);
    json_printf(&outBuf,
                "{command: %d, id:%d,  sender: %Q, payload: %M}",
                data->command,
                data->id,
                data->sender,
                p_planes_resp_print,
                data->payload);
}


#endif //ACMS_SERVER_2JSON_H
