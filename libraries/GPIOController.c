#include "GPIOController.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>


int pinMode(char *path, int isIN){
    int fd, status;
    char direction[50] = "";
    char edge[40] = "";
    strcat(direction, path);
    strcat(edge, path);
    strcat(direction, "direction");
    strcat(edge, "edge");

    fd = open(direction, O_WRONLY);
    if (fd == -1) return EXIT_FAILURE;
    if (isIN) {
        status = write(fd, "in", 2);
    } else {
        status = write(fd, "out", 3);
    }
    close(fd);
    fd = open(edge, O_WRONLY);
    if (fd == -1) return EXIT_FAILURE;
    status = write(fd, "none", 1);
    close(fd);
    return status;
}

int digitalWrite(char *path, int isON){
    int len = -1, fd = -1;
    char value[50] = "";
    strcat(value, path);
    strcat(value, "value");
    
    fd = open(value, O_WRONLY); 
    if (fd == -1) return EXIT_FAILURE;
    if (isON) {
        len = write(fd, "1", 1);
    } else {
        len = write(fd, "0", 1);
    }

    if(len == -1){       
         perror("write");
         close(fd);
         return EXIT_FAILURE;        
     }
     close(fd);
     return EXIT_SUCCESS;
}

int digitalRead(char *path){
    int status = 0, fd = -1;
    char value[50] = "";
    char buf[256];

    strcat(value, path);
    strcat(value, "value");
    
    fd = open(value, O_RDONLY); 
    status= read(fd, buf, 256);

    if(status == -1){       
         perror("write");
         close(fd);
         return EXIT_FAILURE;        
     }
     status = strtol(buf, NULL, 0);
     close(fd);
     return status;
}
