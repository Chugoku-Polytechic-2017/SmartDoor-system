#include "GPIOController.h"

int pinMode(int pin, int isIN){
    int fd, status;
    char *path = "";
    char direction[50] = "";
    char edge[40] = "";
    _createPath(pin,path);
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

int digitalWrite(int pin, int isON){
    int len = -1, fd = -1;
    char *path = "";
    char value[50] = "";
    _createPath(pin,path);
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

int digitalRead(int pin){
    int status = 0, fd = -1;
    char *path = "";
    char value[50] = "";
    char buf[256];
    _createPath(pin,path);

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

//int add_gpio_interrupt()
