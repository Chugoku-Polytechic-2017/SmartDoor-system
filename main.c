#include <stdio.h>
#include "libraries/GPIOController.h"

int main(void) {
    char *path = "/sys/class/gpio/CON9_11/";
    pinMode(path, OUTPUT);
    return;
}