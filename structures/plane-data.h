#ifndef ACMS_SERVER_PLANE_DATA_H
#define ACMS_SERVER_PLANE_DATA_H

typedef struct {
    char id[10];
    char model[10];
    double x;
    double y;
    double height;
    double speed;
    double fuel
} plane_data;

#endif //ACMS_SERVER_PLANE_DATA_H
