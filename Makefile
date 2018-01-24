main: main.o  GPIOController.o  
	arm-linux-gnueabi-gcc -Wall `pkg-config --libs glib-2.0` -o main *.o 

GPIOController.o: libraries/GPIOController.c libraries/atgpio.c
	arm-linux-gnueabi-gcc -Wall `pkg-config --cflags glib-2.0`  -c libraries/GPIOController.c libraries/atgpio.c 

main.o: main.c 
	arm-linux-gnueabi-gcc  -Wall `pkg-config --cflags glib-2.0` -c main.c

clean:
	rm -f *.o main
