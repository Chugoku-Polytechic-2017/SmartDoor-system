
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>


int main(void){
    int fd_red1 = -1, fd_red2 = -1, fd_green1 = -1, fd_green2 = -1, loop, status_code = 0;
    char *path[PORTS];

    path[0] = "/sys/class/gpio/CON9_11/";
    
    pinMode(path[0], 0);

    while (1) {
        digitalWrite(path[0], 0);
        sleep(3);
        digitalWrite(path[0], 1);
        sleep(3);
    }
    return status_code;
}