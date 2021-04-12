#include "oled.h"
#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
int main()
{
    if(wiringPiSetupGpio() == -1)
    {
        printf("wiring init failed\n");
    }
    else
    {
        printf("wiring init succeed\n");
    }
    OLED_Init();
    OLED_ShowString(0,0,"hello",16);
    while(1)
    {
        OLED_ShowString(0,0,"hello",16);
        sleep(1);
        printf("ok\n");
    }
}