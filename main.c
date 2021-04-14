#include "./Oled_driver/oled.h"
#include "./Oled_driver/draw_api.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <string.h>

#define PriTest 1
#define AftTest 0

typedef struct cpu_n
{
    char cpuname[5];
    int cpu_pri_usagetime[7];
    int cpu_pri_usagetimeAll;
    int cpu_atf_usagetime[7];
    int cpu_aft_usagetimeAll;
    int cpu_usageRate;
}cpu_usage;

typedef struct cpu_a
{
    cpu_usage cpu1;
    cpu_usage cpu2;
    cpu_usage cpu3;
    cpu_usage cpu4;
}cpu_all;

cpu_usage cpu_1,cpu_2,cpu_3,cpu_4;
cpu_all CpuUsage;

int temp = 0;
char* TempFilePathCmd = "cat /sys/class/thermal/thermal_zone0/temp";
char* CpuInfoPath = "/proc/stat";

void GetTemp(void);
void GetCpuUasge(void);


void Draw_UI()
{
    OLED_ShowStrRAM(10,0,"Temp:",8);
    Draw_Rect(10,10,110,20,0);
    Draw_Rect(10,23,110,33,0);
    Draw_Rect(10,36,110,46,0);
    Draw_Rect(10,49,110,59,0);
}

void Draw_fillInfo(int temp,cpu_all* CpuUsAge)
{
    OLED_ShowNumRAM(50,0,temp,2,8);
    Draw_Rect(10,10,10+ CpuUsAge->cpu1.cpu_usageRate,20,1);
    Draw_Rect(10,23,10+CpuUsAge->cpu2.cpu_usageRate,33,1);
    Draw_Rect(10,36,10+CpuUsAge->cpu3.cpu_usageRate,46,1);
    Draw_Rect(10,49,10+CpuUsAge->cpu4.cpu_usageRate,59,1);
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
    CpuUsage.cpu1 = cpu_1;
    CpuUsage.cpu2 = cpu_2;
    CpuUsage.cpu3 = cpu_3;
    CpuUsage.cpu4 = cpu_4;
    while(1)
    {
        
        CanvasClear();
        Draw_UI();
        GetTemp();
        GetCpuUasge();
        Draw_fillInfo(temp/1000,&CpuUsage);
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

void GetInfoFromFile(int Ifpri)
{
    FILE *fp;
    char buffer[100];
    fp = fopen(CpuInfoPath,"r");
    if(fp == NULL)
    {
        printf("Open Cpu info file fail\n");
    }
    else
    {
        fgets(buffer,sizeof(buffer),fp);
        if(Ifpri)
        {
            fscanf(fp,"%s %d %d %d %d %d %d %d",CpuUsage.cpu1.cpuname,CpuUsage.cpu1.cpu_pri_usagetime[0],CpuUsage.cpu1.cpu_pri_usagetime[1],CpuUsage.cpu1.cpu_pri_usagetime[2],CpuUsage.cpu1.cpu_pri_usagetime[3],CpuUsage.cpu1.cpu_pri_usagetime[4],CpuUsage.cpu1.cpu_pri_usagetime[5],CpuUsage.cpu1.cpu_pri_usagetime[6]);
            fgets(buffer,sizeof(buffer),fp);
            fscanf(fp,"%s %d %d %d %d %d %d %d",CpuUsage.cpu2.cpuname,CpuUsage.cpu2.cpu_pri_usagetime[0],CpuUsage.cpu2.cpu_pri_usagetime[1],CpuUsage.cpu2.cpu_pri_usagetime[2],CpuUsage.cpu2.cpu_pri_usagetime[3],CpuUsage.cpu2.cpu_pri_usagetime[4],CpuUsage.cpu2.cpu_pri_usagetime[5],CpuUsage.cpu2.cpu_pri_usagetime[6]);
            fgets(buffer,sizeof(buffer),fp);
            fscanf(fp,"%s %d %d %d %d %d %d %d",CpuUsage.cpu3.cpuname,CpuUsage.cpu3.cpu_pri_usagetime[0],CpuUsage.cpu3.cpu_pri_usagetime[1],CpuUsage.cpu3.cpu_pri_usagetime[2],CpuUsage.cpu3.cpu_pri_usagetime[3],CpuUsage.cpu3.cpu_pri_usagetime[4],CpuUsage.cpu3.cpu_pri_usagetime[5],CpuUsage.cpu3.cpu_pri_usagetime[6]);
            fgets(buffer,sizeof(buffer),fp);
            fscanf(fp,"%s %d %d %d %d %d %d %d",CpuUsage.cpu4.cpuname,CpuUsage.cpu4.cpu_pri_usagetime[0],CpuUsage.cpu4.cpu_pri_usagetime[1],CpuUsage.cpu4.cpu_pri_usagetime[2],CpuUsage.cpu4.cpu_pri_usagetime[3],CpuUsage.cpu4.cpu_pri_usagetime[4],CpuUsage.cpu4.cpu_pri_usagetime[5],CpuUsage.cpu4.cpu_pri_usagetime[6]);
            fgets(buffer,sizeof(buffer),fp);
        }
        else
        {
            fscanf(fp,"%s %d %d %d %d %d %d %d",CpuUsage.cpu1.cpuname,CpuUsage.cpu1.cpu_atf_usagetime[0],CpuUsage.cpu1.cpu_atf_usagetime[1],CpuUsage.cpu1.cpu_atf_usagetime[2],CpuUsage.cpu1.cpu_atf_usagetime[3],CpuUsage.cpu1.cpu_atf_usagetime[4],CpuUsage.cpu1.cpu_atf_usagetime[5],CpuUsage.cpu1.cpu_atf_usagetime[6]);
            fgets(buffer,sizeof(buffer),fp);
            fscanf(fp,"%s %d %d %d %d %d %d %d",CpuUsage.cpu2.cpuname,CpuUsage.cpu2.cpu_atf_usagetime[0],CpuUsage.cpu2.cpu_atf_usagetime[1],CpuUsage.cpu2.cpu_atf_usagetime[2],CpuUsage.cpu2.cpu_atf_usagetime[3],CpuUsage.cpu2.cpu_atf_usagetime[4],CpuUsage.cpu2.cpu_atf_usagetime[5],CpuUsage.cpu2.cpu_atf_usagetime[6]);
            fgets(buffer,sizeof(buffer),fp);
            fscanf(fp,"%s %d %d %d %d %d %d %d",CpuUsage.cpu3.cpuname,CpuUsage.cpu3.cpu_atf_usagetime[0],CpuUsage.cpu3.cpu_atf_usagetime[1],CpuUsage.cpu3.cpu_atf_usagetime[2],CpuUsage.cpu3.cpu_atf_usagetime[3],CpuUsage.cpu3.cpu_atf_usagetime[4],CpuUsage.cpu3.cpu_atf_usagetime[5],CpuUsage.cpu3.cpu_atf_usagetime[6]);
            fgets(buffer,sizeof(buffer),fp);
            fscanf(fp,"%s %d %d %d %d %d %d %d",CpuUsage.cpu4.cpuname,CpuUsage.cpu4.cpu_atf_usagetime[0],CpuUsage.cpu4.cpu_atf_usagetime[1],CpuUsage.cpu4.cpu_atf_usagetime[2],CpuUsage.cpu4.cpu_atf_usagetime[3],CpuUsage.cpu4.cpu_atf_usagetime[4],CpuUsage.cpu4.cpu_atf_usagetime[5],CpuUsage.cpu4.cpu_atf_usagetime[6]);
            fgets(buffer,sizeof(buffer),fp);
        }
    }
    fclose(fp);
}

void GetCpuUasge()
{
    int count=0;
    memset(cpu_1,0,sizeof(cpu_1));
    memset(cpu_2,0,sizeof(cpu_2));
    memset(cpu_3,0,sizeof(cpu_3));
    memset(cpu_4,0,sizeof(cpu_4));
    GetInfoFromFile(PriTest);
    delay(100);
    GetInfoFromFile(AftTest);
    for(count=0;count<=6;count++)
    {
        CpuUsage.cpu1.cpu_pri_usagetimeAll = CpuUsage.cpu1.cpu_pri_usagetimeAll + CpuUsage.cpu1.cpu_pri_usagetime[i+1];
        CpuUsage.cpu1.cpu_aft_usagetimeAll = CpuUsage.cpu1.cpu_aft_usagetimeAll + CpuUsage.cpu1.cpu_atf_usagetime[i+1];

        CpuUsage.cpu2.cpu_pri_usagetimeAll = CpuUsage.cpu2.cpu_pri_usagetimeAll + CpuUsage.cpu2.cpu_pri_usagetime[i+1];
        CpuUsage.cpu2.cpu_aft_usagetimeAll = CpuUsage.cpu2.cpu_aft_usagetimeAll + CpuUsage.cpu2.cpu_atf_usagetime[i+1];

        CpuUsage.cpu3.cpu_pri_usagetimeAll = CpuUsage.cpu3.cpu_pri_usagetimeAll + CpuUsage.cpu3.cpu_pri_usagetime[i+1];
        CpuUsage.cpu3.cpu_aft_usagetimeAll = CpuUsage.cpu3.cpu_aft_usagetimeAll + CpuUsage.cpu3.cpu_atf_usagetime[i+1];

        CpuUsage.cpu4.cpu_pri_usagetimeAll = CpuUsage.cpu4.cpu_pri_usagetimeAll + CpuUsage.cpu4.cpu_pri_usagetime[i+1];
        CpuUsage.cpu4.cpu_aft_usagetimeAll = CpuUsage.cpu4.cpu_aft_usagetimeAll + CpuUsage.cpu4.cpu_atf_usagetime[i+1];
    }

    CpuUsage.cpu1.cpu_usageRate = 100 - (100*(CpuUsage.cpu1.cpu_atf_usagetime[3]-CpuUsage.cpu1.cpu_pri_usagetime[3])/(CpuUsage.cpu1.cpu_aft_usagetimeAll - CpuUsage.cpu1.cpu_pri_usagetimeAll));
    CpuUsage.cpu2.cpu_usageRate = 100 - (100*(CpuUsage.cpu2.cpu_atf_usagetime[3]-CpuUsage.cpu2.cpu_pri_usagetime[3])/(CpuUsage.cpu2.cpu_aft_usagetimeAll - CpuUsage.cpu2.cpu_pri_usagetimeAll));
    CpuUsage.cpu3.cpu_usageRate = 100 - (100*(CpuUsage.cpu3.cpu_atf_usagetime[3]-CpuUsage.cpu3.cpu_pri_usagetime[3])/(CpuUsage.cpu3.cpu_aft_usagetimeAll - CpuUsage.cpu3.cpu_pri_usagetimeAll));
    CpuUsage.cpu4.cpu_usageRate = 100 - (100*(CpuUsage.cpu4.cpu_atf_usagetime[3]-CpuUsage.cpu4.cpu_pri_usagetime[3])/(CpuUsage.cpu4.cpu_aft_usagetimeAll - CpuUsage.cpu4.cpu_pri_usagetimeAll));

    
}
