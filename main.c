#include <stdio.h>
#include "libraries/GPIOController.h"
#include "libraries/capture.h"


//FALSE = 人がいる
//TRUE = 人がいない

// 人感センサーの構造体
typedef struct human_sensor_struct {
    gboolean sensor;
    gboolean is_authenticate; 
    gboolean LED; 
    guint timeout;
} human_sensor;

gboolean face_authenticate(gpointer data)
{
    human_sensor *sensor = (human_sensor *)data;
    int status = 0;
    delay(20);
    sensor->sensor = digitalRead(13);
    printf("timer intterupt %d\n", sensor->sensor);
    //早期リターン
    if(sensor->sensor == TRUE) {
        sensor->is_authenticate = FALSE;
        return FALSE;
    }

    printf("starting capture\n");
    caputure();
    printf("starting face detect\n");
    status = system("python2.7 python/facer.py authenticate -d sample01");
    printf("%d\n", status);

    switch(status) {
        case 512:
            sensor->is_authenticate = FALSE;
            printf("検出に失敗しました。\n");
            return FALSE;
        case 768:
            sensor->is_authenticate = FALSE;
            printf("認証できませんでした。\n");
            return FALSE;
        default:
            break;
    }
    digitalWrite(11, sensor->LED);
    sensor->LED = !sensor->LED;
    sensor->is_authenticate = FALSE;
    return FALSE;
}

static gboolean interrupt_human_sensor(GIOChannel *ch, gpointer d) {
    human_sensor *sensor = (human_sensor *)d;
    sensor->sensor = !sensor->sensor;
    //チャタリング除去
    delay(10);
    sensor->sensor = digitalRead(13);
    //センサーがHighまたは、認証中だったら早期リターン
    if(sensor->sensor == TRUE || sensor->is_authenticate == TRUE) {
        g_io_channel_read_to_end(ch, NULL, NULL, NULL);
        return TRUE;
    }

    //センサーが反応して、認証中でない場合
    if(sensor->sensor == FALSE && sensor->is_authenticate == FALSE) {
        sensor->is_authenticate = TRUE;
        printf("calling face_authenticate function\n");
        sensor->timeout = g_timeout_add_seconds(3, face_authenticate, sensor);
    }
    g_io_channel_read_to_end(ch, NULL, NULL, NULL);
    return TRUE;
}

static gboolean interrupt_pyroelectric_sensor(GIOChannel *ch, gpointer d) {
    printf("interrupt_pyroelectric_sensor\n");
    human_sensor *sensor = (human_sensor *)d;
    sensor->sensor = digitalRead(13);
    if(digitalRead(15) != HIGH) {
        g_io_channel_read_to_end(ch, NULL, NULL, NULL);
        return TRUE;
    }
    //感センサーが反応していない状態
    //中から外へでる場合
    if ( sensor->sensor == TRUE) {
        printf("中から外へ\n");
        digitalWrite(11, HIGH);
        delay(90);
        digitalWrite(11, LOW);
    }
    //人感センサーが反応している状態
    //外から中へ入る場合
    else if (sensor->sensor == FALSE) {
        digitalWrite(11, LOW);
        printf("外から中へ\n");
    }
    g_io_channel_read_to_end(ch, NULL, NULL, NULL);
    return TRUE;
}

int main(void){
    int status;
    GMainLoop *loop;
    human_sensor sensor;
    sensor.is_authenticate = FALSE;
    sensor.LED = TRUE;
 
    loop = g_main_loop_new(NULL, FALSE);
    status = pinMode(11, OUTPUT);
    status = pinMode(13, INPUT);
    digitalWrite(11, 0);
    sensor.sensor = digitalRead(13);
    at_gpio_add(13,  AT_GPIO_EDGE_BOTH, interrupt_human_sensor, &sensor,  NULL);
    at_gpio_add(15,  AT_GPIO_EDGE_BOTH, interrupt_pyroelectric_sensor, &sensor,  NULL);
    g_main_loop_run(loop);
    g_main_loop_unref(loop);
 
    return status;
} 
