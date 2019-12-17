#ifndef ACMS_SERVER_SCHEDULE_H
#define ACMS_SERVER_SCHEDULE_H

typedef struct {
    char src[3];
    char dst[3];
    unsigned long dept_time;
    unsigned long arr_time;
    int status;
} schedule_data;

#endif //ACMS_SERVER_SCHEDULE_H
