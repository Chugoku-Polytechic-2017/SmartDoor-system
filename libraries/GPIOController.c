#include "GPIOController.h"

int pinMode(int pin, int isIN){
    int fd, status;
    GString *s;
    s = g_string_new(NULL);
    char *direction = "";
    char *edge = "";

    g_string_printf(s, AT_GPIO_PATH_DIRECTION, (guint)pin);
    direction = s->str;
    g_string_printf(s, AT_GPIO_PATH_EDGE, (guint)pin);
    edge = s->str;

    fd = open(direction, O_WRONLY);
    if (fd == -1) return EXIT_FAILURE;
    if (isIN) {
        status = write(fd, "in", 2);
    } else {
        status = write(fd, "out", 3);
    }
    close(fd);
    fd = open(edge, O_WRONLY);
    if (fd == -1) return EXIT_FAILURE;
    status = write(fd, "none", 4);
    close(fd);
    return status;
}

int digitalWrite(int pin, int isON){
    int len = -1, fd = -1;
    GString *s;
    char *value = "";
    s = g_string_new(NULL);
    g_string_printf(s, AT_GPIO_PATH_VALUE, (guint)pin);
    value = s->str;
    fd = open(value, O_WRONLY); 
    if (fd == -1) return EXIT_FAILURE;
    if (isON) {
        len = write(fd, "1", 1);
    } else {
        len = write(fd, "0", 1);
    }

    if(len == -1){       
         perror("write");
         close(fd);
         return EXIT_FAILURE;        
     }
     close(fd);
     return EXIT_SUCCESS;
}

int digitalRead(int pin){
    int status = 0, fd = -1;
    GString *s;
    char *value = "";
    char buf[256];
    s = g_string_new(NULL);
    g_string_printf(s, AT_GPIO_PATH_VALUE, (guint)pin);
    value = s->str;
    
    fd = open(value, O_RDONLY); 
    status= read(fd, buf, 256);

    if(status == -1){       
         perror("write");
         close(fd);
         return EXIT_FAILURE;        
     }
     status = strtol(buf, NULL, 0);
     close(fd);
     return status;
}

//int add_gpio_interrupt()
