#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>

#define OUTPUT 0
#define INPUT 1
#define HIGH 1
#define LOW 0
#define COM9_11 "/sys/class/gpio/CON9_11/"
#define COM9_12 "/sys/class/gpio/CON9_12/"
#define COM9_13 "/sys/class/gpio/CON9_13/"
#define COM9_14 "/sys/class/gpio/CON9_14/"
#define COM9_15 "/sys/class/gpio/CON9_15/"
#define COM9_16 "/sys/class/gpio/CON9_16/"
#define COM9_17 "/sys/class/gpio/CON9_17/"
#define COM9_18 "/sys/class/gpio/CON9_18/"

extern int pinMode(int pin, int isIN);
extern int digitalWrite(int pin, int isON);
extern int digitalRead(int pin);
