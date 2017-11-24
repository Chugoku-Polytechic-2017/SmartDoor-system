#include <stdio.h>
#include "GPIOController.c"

void main(void) {
    char *path = "/sys/class/gpio/CON9_11/";
    pinMode(path, OUTPUT);
    return;
}