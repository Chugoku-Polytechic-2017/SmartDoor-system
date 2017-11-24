#ifndef GPIOController_H
#define GPIOController_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>

#define OUTPUT 1
#define INPUT 0

int pinMode(char *pin, int mode);

#endif