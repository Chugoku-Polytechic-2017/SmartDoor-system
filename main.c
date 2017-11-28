#include <stdio.h>
#include "libraries/GPIOController.h"

int main(void) {
    int status;
    char *path = "/sys/class/gpio/CON9_11/";
    status = pinMode(path, OUTPUT);
    if (status != 0) {
        printf("error\r\n");
        return -1;
    }
    while(1){
        digitalWrite(path, HIGH);
        sleep(3);
        digitalWrite(path, LOW);
        sleep(3);
    }
    return;
}