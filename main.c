#include <stdio.h>
#include "libraries/GPIOController.h"

int main(void) {
    char *path = "/sys/class/gpio/CON9_11/";
    pinMode(path, OUTPUT);
    while(1){
        digitalWrite(path, HIGH);
        sleep(3);
        digitalWrite(path, LOW);
        sleep(3);
    }
    return;
}