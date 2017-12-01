/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>

#define OUTPUT 1
#define INPUT 0
#define HIGH 1
#define LOW 0

int pinMode(char *pin, int mode);
int digitalWrite(char *path, int isON);

int main(void) {
    int status;
    char *path = "/sys/class/gpio/CON9_11/";
    status = pinMode(path, OUTPUT);
    if (status == -1) {
        printf("error\r\n");
        return -1;
    }

    while(1){
        status = digitalWrite(path, HIGH);
        printf("%d",status);
        sleep(3);
        status = digitalWrite(path, LOW);
        printf("%d",status);
        sleep(3);
    }
    return;
}

int pinMode(char *path, int isIN){
    int fd, status;
    char direction[50] = "";
    char edge[40] = "";
    strcat(direction, path);
    strcat(edge, path);
    strcat(direction, "direction");
    strcat(edge, "edge");

    fd = open(direction, O_WRONLY);
    if (fd == -1) {
        perror("open");
        return EXIT_FAILURE;
    }
    if (isIN) {
        status = write(fd, "in", 2);
    } else {
        status = write(fd, "out", 3);
    }
    close(fd);
    fd = open(edge, O_WRONLY);
    if (fd == -1) {
        printf("error2");
        return EXIT_FAILURE;
        }
    status = write(fd, "none", 4);
    close(fd);
    printf("error5\n%d\n",status);
    return status;
}

int digitalWrite(char *path, int isON){
    int len = -1, fd = -1;
    char value[50] = "";
    strcat(value, path);
    strcat(value, "value");
    
    fd = open(value, O_WRONLY); 
    if (fd == -1){
        printf("error3");
        return EXIT_FAILURE;
    } 
    if (isON) {
        len = write(fd, "1", 1);
    } else {
        len = write(fd, "0", 1);
    }

    if(len == -1){       
         perror("write");
         close(fd);
         printf("error4");
         return EXIT_FAILURE;        
     }
     close(fd);
     printf("success");
     return EXIT_SUCCESS;
}
*/
/*
#include <string.h>
#include <fcntl.h>*/
#include "libraries/GPIOController.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>

#define OUTPUT 1
#define INPUT 0
#define HIGH 1
#define LOW 0

//int pinMode(char *pin, int mode);

#define PORTS 8

//int get_dip_status(char *path);



int main(void){
    int fd_red1 = -1, fd_red2 = -1, fd_green1 = -1, fd_green2 = -1, loop, status_code = 0;
    char *path[PORTS];

    path[0] = "/sys/class/gpio/CON9_11/";
    path[1] = "/sys/class/gpio/CON9_12/";
    path[2] = "/sys/class/gpio/CON9_13/";
    path[3] = "/sys/class/gpio/CON9_14/";
    path[4] = "/sys/class/gpio/CON9_15/";
    path[5] = "/sys/class/gpio/CON9_16/";
    path[6] = "/sys/class/gpio/CON9_17/";
    path[7] = "/sys/class/gpio/CON9_18/";
    
    pinMode(path[0], OUTPUT);
    
    while (1) {
        digitalWrite(path[0], HIGH);
        sleep(3);
        digitalWrite(path[0], LOW);
        sleep(3);
    }
}




/*
int get_dip_status(char *path){
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
*/