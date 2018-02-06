#include <stdio.h>
#include "libraries/GPIOController.h"
#include "libraries/capture.h"

gboolean human_sensor = FALSE; 
gboolean LED = TRUE; 

gboolean callback(gpointer data)
{
    int status = 0;
    printf("timer intterupt %d\n", human_sensor);
    //早期リターン
    if(human_sensor == FALSE) {
        return FALSE;
    }
    printf("starting capture\n");
    caputure();
    printf("starting face detect\n");
    status = system("python2.7 python/facer.py authenticate -d sample01");
    switch(status) {
        case 2:
            printf("検出に失敗しました。\n");
            return FALSE;
        case 3:
            printf("認証できませんでした。\n");
            return FALSE;
        default:
            break;
    }
    digitalWrite(11, LED);
    LED = !LED;
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
    int status;
    GMainLoop *loop;
 
    loop = g_main_loop_new(NULL, FALSE);
    status = pinMode(11, OUTPUT);
    at_gpio_add(13,  AT_GPIO_EDGE_BOTH, on_push, NULL,  NULL);
    g_main_loop_run(loop);
    g_main_loop_unref(loop);
 
    return status;
} 
