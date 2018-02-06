#include <stdio.h>
#include "libraries/GPIOController.h"
#include "libraries/capture.h"

gboolean human_sensor = FALSE; 
gboolean LED = TRUE; 

gboolean callback(gpointer data)
{
    int status = 0;
    char command[50] = "python2.7 ./facer.py -h";
    printf("timer intterupt %d\n", human_sensor);
    //早期リターン
    if(human_sensor == FALSE) {
        return FALSE;
    }

    digitalWrite(11, LED);
    LED = !LED;
    caputure();
    status = system(command);
    if(status == -1){
         printf("実行できませんでした\n");
    }
    return FALSE;
}

static gboolean on_push(GIOChannel *ch, gpointer d) {
    printf("on_push %d\r\n", human_sensor);
    human_sensor = !human_sensor;
    if (human_sensor == TRUE) {
        g_timeout_add_seconds(3, callback, NULL);
    }

    g_io_channel_read_to_end(ch, NULL, NULL, NULL);
    return TRUE;
}

int main(void){
   char command[50] = "python2.7 ./facer.py -h";
    int status, flag = 0;
     GMainLoop *loop;

    status = caputure();
    if(system(command)==-1){
        printf("実行できませんでした\n");
    }
    loop = g_main_loop_new(NULL, FALSE);
    status = pinMode(11, OUTPUT);
    at_gpio_add(13,  AT_GPIO_EDGE_FALLING, on_push, &flag,  NULL);
    g_timeout_add_seconds(1, callback, NULL);
    g_main_loop_run(loop);
    g_main_loop_unref(loop);

    return status;
} 
