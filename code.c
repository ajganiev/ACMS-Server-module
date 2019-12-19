#include <stdio.h>
#include <stdlib.h>
#include "frozen.c"

#define PLANE_ID_SIZE 30
struct planeData {
    char uuid[PLANE_ID_SIZE];
    double velocity[3];
    double position[3];
};

typedef struct {
    char uuid[30];
    double velocity[3];
    double position[3];
    double fuel;
    char path_id[20];
} p_plane_data;


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
    p_planes *p = va_arg(*ap, p_auth_resp *);
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



int main(int argc, char **argv) {
    // str has the following JSON string - array of objects:
    // { "a": [ {"b": 123}, {"b": 345} ] }
    struct planeData airplane = {
            "ICE-01",
            {4, 5, 6},
            {7, 8, 9}
    };
    printf("%s \n", airplane.uuid);
    char str[2048];
    struct planeData airplane2;
    p_planes_resp2json(str, testData)
    printf("%f, %f, %f \n", airplane2.velocity[0], airplane2.velocity[1], airplane2.velocity[2]);
    printf("%f, %f, %f \n", airplane2.position[0], airplane2.position[1], airplane2.position[2]);
    printf("%s\n", airplane2.uuid);

//    for (i = 0; json_scanf_array_elem(str, len, ".a", i, &t) > 0; i++) {
//        // t.type == JSON_TYPE_OBJECT
//        json_scanf(t.ptr, t.len, "{b: %d}", &value);  // value is 123, then 345
//    }


}
