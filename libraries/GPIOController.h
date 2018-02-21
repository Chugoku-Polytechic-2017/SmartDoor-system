#ifndef __GPIO_CONTROLLER_H__
#define __GPIO_CONTROLLER_H__

#include "atgpio.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <glib.h>

#define OUTPUT 0
#define INPUT 1
#define HIGH 1
#define LOW 0

void delay(unsigned int howLong);
int pinMode(int pin, int isIN);
int digitalWrite(int pin, int isON);
int digitalRead(int pin);

#endif /* __GPIO_CONTROLLER_H__ */
