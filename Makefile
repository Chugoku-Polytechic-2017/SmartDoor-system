main: main.o GPIOController.o
	arm-linux-gnueabi-gcc -o main main.o GPIOController.o
mian.o: main.c
	arm-linux-gnueabi-gcc -c main.c
GPIOController.o: libraries/GPIOController.c
	arm-linux-gnueabi-gcc -c libraries/GPIOController.c
clean:
	rm -f main main.o GPIOController.o
