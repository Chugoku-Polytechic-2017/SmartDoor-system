
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include "libraries/GPIOController.h"

int main(void){
    int fd_red1 = -1, fd_red2 = -1, fd_green1 = -1, fd_green2 = -1, loop, status_code = 0;
    char *path[2];
    int status;
    
    path[0] = COM9_11;
    path[1] = COM9_13;
    
    pinMode(path[0], OUTPUT);
    pinMode(path[1], INPUT);

    while (1) {
        status = digitalRead(path[1]);
        if(status == 1)                         //負論理
            digitalWrite(path[0],LOW);
        else
            digitalWrite(path[0],HIGH);
    }
    return status_code;
} 
