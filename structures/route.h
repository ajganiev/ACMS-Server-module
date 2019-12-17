#ifndef ACMS_SERVER_ROUTE_H
#define ACMS_SERVER_ROUTE_H

#define MAX_ROUTE 15

typedef struct {
    char src[3];
    char dst[3];
    double x[MAX_ROUTE];
    double y[MAX_ROUTE];
    short int changed;
} route;

#endif //ACMS_SERVER_ROUTE_H
