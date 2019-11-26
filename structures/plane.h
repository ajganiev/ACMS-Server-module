#ifndef ACMS_SERVER_PLANE_H
#define ACMS_SERVER_PLANE_H
#define ROUTE_NODES 15
#define UUID_SIZE 10;

/**
 * I'm not sure about defenition of plane data structure in Server module
 * Because server act just as re-transmitter between Airplane module and Client
 **/

/**
 * Plane data structure
 * size: 8 * 7 + 10 * 2 + 8 * 15 = 196 bytes
 */
struct Plane {
    char uuid[UUID_SIZE];
    double speed;
    double x, y;
    double altitude;
    double fuel;
    double cabin_pressure;
    double nose_tilt;
    double cur_route[ROUTE_NODES];
};

#endif //ACMS_SERVER_PLANE_H
