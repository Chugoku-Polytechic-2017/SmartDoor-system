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

    caputure();
    status = system("python2.7 ./facer.py detect face.jpg -d");
    if(status == -1){
         printf("検出または実行に失敗しました。\n");
    }
    status = system("python2.7 ./facer.py identify -j -d sample01");
    if(status == -1){
         printf("認証に失敗しました。\n");
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
