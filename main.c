
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include "libraries/GPIOController.h"

int main(void){
    int fd_red1 = -1, fd_red2 = -1, fd_green1 = -1, fd_green2 = -1, loop, status_code = 0;
    char *path[PORTS];
    int status;
    
    path[0] = COM9_11;
    
    pinMode(path[0], OUTPUT);

    while (1) {
        digitalWrite(path[0], LOW);
        status = digitalRead(path[0]);
        printf("%d\r\n",status);
        sleep(3);
        digitalWrite(path[0], HIGH);
        status = digitalRead(path[0]);
        printf("%d\r\n",status);
        sleep(3);
    }
    return status_code;
} 
