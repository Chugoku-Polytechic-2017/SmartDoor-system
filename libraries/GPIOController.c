#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>

int pinMode(char *path, int isIN){
    int fd, status;
    char direction[50] = "";
    char edge[40] = "";
    strcat(direction, path);
    strcat(edge, path);
    strcat(direction, "direction");
    strcat(edge, "edge");

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
    status = write(fd, "none", 1);
    close(fd);
    return status;
}