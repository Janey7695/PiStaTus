#include "./Oled_driver/oled.h"
#include "./Oled_driver/draw_api.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

int temp = 0;
char* TempFilePathCmd = "cat /sys/class/thermal/thermal_zone0/temp";
void GetTemp(void);
void Draw_UI()
{
    OLED_ShowStrRAM(10,0,"Temp:",8);
    Draw_Rect(10,10,110,20,0);
    Draw_Rect(10,23,110,33,0);
    Draw_Rect(10,36,110,46,0);
    Draw_Rect(10,49,110,59,0);
}

void Draw_fillInfo(int temp,int cpu1Usage,int cpu2Usage,int cpu3Usage,int cpu4Usage)
{
    OLED_ShowNumRAM(50,0,temp,2,8);
    Draw_Rect(10,10,10+cpu1Usage,20,1);
    Draw_Rect(10,23,10+cpu2Usage,33,1);
    Draw_Rect(10,36,10+cpu3Usage,46,1);
    Draw_Rect(10,49,10+cpu4Usage,59,1);
}

int main()
{   
    int i = 0;
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
        
        CanvasClear();
        Draw_UI();
	GetTemp();
        Draw_fillInfo(temp/1000,6+i,7+2*i,3+i,10-i);
        DisPlay();
        sleep(1);
        i++;
        if(i==10)
            i = 0;
        printf("ok\n");
    }
}

void GetTemp(void)
{
    FILE *fp;
    char buffer[20];
    fp = popen(TempFilePathCmd,"r");
    if(fp!=NULL)
    {
    fgets(buffer,sizeof(buffer),fp);
    temp = atoi(buffer);
    printf("now tempture is %.1f 'C\n",temp/1000.0);
    }
    else
    {
	    printf("read tempture fail\n");
    }
    pclose(fp);
} 
