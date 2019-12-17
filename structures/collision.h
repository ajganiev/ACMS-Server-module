#ifndef ACMS_SERVER_COLLISION_H
#define ACMS_SERVER_COLLISION_H

#define PLANE_ID_SIZE 10

typedef struct {
    char a[PLANE_ID_SIZE];
    char b[PLANE_ID_SIZE];
    int x;
    int y;
    double radius;
} collision_data;

#endif //ACMS_SERVER_COLLISION_H
