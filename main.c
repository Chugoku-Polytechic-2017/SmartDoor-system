
#include <stdio.h>
#include "libraries/GPIOController.h"

int main(void){
    int status;
    status = pinMode(11, OUTPUT);
    status = pinMode(13, INPUT);

    while (1) {
        status = digitalRead(13);
        printf("%d\r\n",status);
        if(status == 0) {                        //負論理
            digitalWrite(11,LOW);
            sleep(1);
        }else{
            digitalWrite(11,HIGH); 
            sleep(1);        
        }
    }
    return status;
} 
