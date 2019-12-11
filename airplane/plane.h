#ifndef ACMS_SERVER_PLANE_H
#define ACMS_SERVER_PLANE_H

#define PLANE_ID_SIZE 10
#define MAX_ROUTE_SIZE 10
typedef struct {
    char uuid[PLANE_ID_SIZE];
    double speed;
    double x;
    double y;
    double height
} plane_data;

typedef struct {
    double route_x[MAX_ROUTE_SIZE];
    double route_y[MAX_ROUTE_SIZE];
};


#endif //ACMS_SERVER_PLANE_H
