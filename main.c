#include <stdio.h>
#include "libraries/GPIOController.h"

static gboolean on_push(GIOChannel *ch, gpointer d) {
    int *flag;
    flag = d;
    printf("on_push \r\n");
    if (*flag == 0)
        digitalWrite(11, HIGH);
    else
        digitalWrite(11, LOW);

    *flag = ~*flag;

    g_io_channel_read_to_end(ch, NULL, NULL, NULL);
    return TRUE;
}

int main(void){
    int status, flag = 0;
    GMainLoop *loop;

    loop = g_main_loop_new(NULL, FALSE);
    status = pinMode(11, OUTPUT);
    at_gpio_add(13,  AT_GPIO_EDGE_FALLING, on_push, &flag,  NULL);
    g_timeout_add_seconds(1, callback, NULL);
    g_main_loop_run(loop);
    g_main_loop_unref(loop);

    return status;
} 
