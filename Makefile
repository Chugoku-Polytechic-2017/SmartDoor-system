main.arm: main.o  GPIOController.o  
	arm-linux-gnueabi-gcc -Wall `pkg-config --libs glib-2.0` -o main.arm *.o 

GPIOController.o: libraries/GPIOController.c libraries/atgpio.c
	arm-linux-gnueabi-gcc -Wall `pkg-config --cflags glib-2.0`  -c libraries/GPIOController.c libraries/atgpio.c 

capture.o: libraries/capture.c
	arm-linux-gnueabi-gcc -Wall -ljpeg -c libraries/capture.c

main.o: main.c 
	arm-linux-gnueabi-gcc  -Wall `pkg-config --cflags glib-2.0` -c main.c

clean:
	rm -f *.o main
