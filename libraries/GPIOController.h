#ifndef GPIOController_H
#define GPIOController_H

#define OUTPUT 1
#define INPUT 0
#define HIGH 1
#define LOW 0

int pinMode(char *pin, int mode);
int digitalWrite(char *path, int isON);

#endif