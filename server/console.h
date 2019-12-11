#ifndef ACMS_CONSOLE_H
#define ACMS_CONSOLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int read_console(char *read_buffer, size_t max_len)
{
    memset(read_buffer, 0, max_len);
    ssize_t read_count = 0;
    ssize_t total_read = 0;
    do {
        read_count = read(STDIN_FILENO, read_buffer, max_len);
        if (read_count < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
            perror("read()");
            return -1;
        }
        else if (read_count < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
            break;
        }
        else if (read_count > 0) {
            total_read += read_count;
            if (total_read > max_len) {
                printf("[ACMS] Chopping message.\n");
                fflush(STDIN_FILENO);
                break;
            }
        }
    } while (read_count > 0);
    size_t len = strlen(read_buffer);
    if (len > 0 && read_buffer[len - 1] == '\n')
        read_buffer[len - 1] = '\0';
    return 0;
}

#endif //ACMS_CONSOLE_H
