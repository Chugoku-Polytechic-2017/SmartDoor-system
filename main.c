
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <poll.h>
#include "libraries/GPIOController.h"

int main(void){
//    char *path[2];
    int status;
    
//    path[0] = COM9_11;
//    path[1] = COM9_13;
    
    pinMode(11, OUTPUT);
    pinMode(13, INPUT);

//    enum atgpioPin pin;
//    pin = COM9_11;

    while (1) {
        status = digitalRead(11);
        if(status == 0) {                        //負論理
            digitalWrite(11,LOW);
            printf("%d\r\n",status);
            sleep(1);
        }else{
            digitalWrite(11,HIGH); 
            printf("%d\r\n",status);
            sleep(1);        
        }
    }
    return status;
} 
