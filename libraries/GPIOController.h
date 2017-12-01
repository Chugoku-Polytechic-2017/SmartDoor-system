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
#define PORTS 8

extern int pinMode(char *pin, int mode);
extern int digitalWrite(char *path, int isON);