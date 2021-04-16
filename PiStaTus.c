#include "./Oled_driver/oled.h"
#include "./Oled_driver/draw_api.h"
#include "./Oled_driver/img.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <string.h>

#define PriTest 1
#define AftTest 0
#define UsageRectBeigin_x 1
#define UsageRectBeigin_y 35
#define UsageRectWidth 5
#define UsageRectLength 80
#define UsageRectInter 2

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
    cpu_usage cpu[4];
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
    
    OledPaint.Draw.Picture(0,0,38,28,cpu_temp_img);
    for(int i=0;i<4;i++)
    {
        OledPaint.Draw.Rect(UsageRectBeigin_x,UsageRectBeigin_y+(UsageRectWidth+UsageRectInter)*i,UsageRectBeigin_x+UsageRectLength,UsageRectWidth+UsageRectBeigin_y+(UsageRectWidth+UsageRectInter)*i,0);
    }
}

void Draw_fillInfo(int temp,cpu_all* CpuUsAge)
{
    float usgRate=0.0;
    int usgRateLen =0;
    OledPaint.Show.Number(42,0,temp,2,16);
    for(int countCpuNum=0;countCpuNum<4;countCpuNum++)
    {
        usgRate = CpuUsAge->cpu[countCpuNum].cpu_usageRate/100.0;
        usgRateLen = usgRate * UsageRectLength;
        OledPaint.Draw.Rect(UsageRectBeigin_x,UsageRectBeigin_y+(UsageRectWidth+UsageRectInter)*countCpuNum,UsageRectBeigin_x+ usgRateLen,UsageRectWidth+UsageRectBeigin_y+(UsageRectWidth+UsageRectInter)*countCpuNum,1);
    }
    
}

int main()
{   
    int i = 0;
    if(wiringPiSetupGpio() == -1)
    {
        printf("wiring init failed\n");
        exit(1);
    }
    else
    {
        printf("wiring init succeed\n");
    }
    Oled_DrawApi_Init();
    CpuUsage.cpu[0] = cpu_1;
    CpuUsage.cpu[1] = cpu_2;
    CpuUsage.cpu[2] = cpu_3;
    CpuUsage.cpu[3] = cpu_4;
    while(1)
    {
        
        CanvasClear();
        Draw_UI();
        GetTemp();
        GetCpuUasge();
        Draw_fillInfo(temp/1000,&CpuUsage);
        DisPlay();
        delay(300);
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
        exit(1);
    }
    else
    {
        printf("Open Cpu info file succ\n");
        fgets(buffer,sizeof(buffer),fp); //move fileptr to second line 
        if(Ifpri)
        {
            for(int countCpuNum=0;countCpuNum<4;countCpuNum++)
            {
                fscanf(fp,"%s %d %d %d %d %d %d %d",&CpuUsage.cpu[countCpuNum].cpuname,&CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[0],&CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[1],&CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[2],&CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[3],&CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[4],&CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[5],&CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[6]);
                printf("%d %d %d %d %d %d %d\n",CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[0],CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[1],CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[2],CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[3],CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[4],CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[5],CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[6]);
                fgets(buffer,sizeof(buffer),fp);
            }
            printf("read all pri_cpuinfo .. \n");
        }
        else
        {
            for(int countCpuNum=0;countCpuNum<4;countCpuNum++)
            {
                fscanf(fp,"%s %d %d %d %d %d %d %d",&CpuUsage.cpu[countCpuNum].cpuname,&CpuUsage.cpu[countCpuNum].cpu_atf_usagetime[0],&CpuUsage.cpu[countCpuNum].cpu_atf_usagetime[1],&CpuUsage.cpu[countCpuNum].cpu_atf_usagetime[2],&CpuUsage.cpu[countCpuNum].cpu_atf_usagetime[3],&CpuUsage.cpu[countCpuNum].cpu_atf_usagetime[4],&CpuUsage.cpu[countCpuNum].cpu_atf_usagetime[5],&CpuUsage.cpu[countCpuNum].cpu_atf_usagetime[6]);
                printf("%d %d %d %d %d %d %d\n",CpuUsage.cpu[countCpuNum].cpu_atf_usagetime[0],CpuUsage.cpu[countCpuNum].cpu_atf_usagetime[1],CpuUsage.cpu[countCpuNum].cpu_atf_usagetime[2],CpuUsage.cpu[countCpuNum].cpu_atf_usagetime[3],CpuUsage.cpu[countCpuNum].cpu_atf_usagetime[4],CpuUsage.cpu[countCpuNum].cpu_atf_usagetime[5],CpuUsage.cpu[countCpuNum].cpu_atf_usagetime[6]);
                fgets(buffer,sizeof(buffer),fp);
            }
            printf("read all aft_cpuinfo ..\n");
        }
    }
    fclose(fp);
}

void GetCpuUasge()
{
    int count=0;
    int countCpuNum =0;
    printf("begin to clear cpuinfo\n");
    memset(&cpu_1,0,sizeof(cpu_1));
    memset(&cpu_2,0,sizeof(cpu_2));
    memset(&cpu_3,0,sizeof(cpu_3));
    memset(&cpu_4,0,sizeof(cpu_4));
    GetInfoFromFile(PriTest);
    delay(100);
    GetInfoFromFile(AftTest);
    for(countCpuNum=0;countCpuNum<4;countCpuNum++)
    {
        for(count=0;count<=6;count++)
        {
            CpuUsage.cpu[countCpuNum].cpu_pri_usagetimeAll = CpuUsage.cpu[countCpuNum].cpu_pri_usagetimeAll + CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[count];
            CpuUsage.cpu[countCpuNum].cpu_aft_usagetimeAll = CpuUsage.cpu[countCpuNum].cpu_aft_usagetimeAll + CpuUsage.cpu[countCpuNum].cpu_atf_usagetime[count];

        }
        CpuUsage.cpu[countCpuNum].cpu_usageRate = 100 - (100*1.0*(CpuUsage.cpu[countCpuNum].cpu_atf_usagetime[3]-CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[3])/(CpuUsage.cpu[countCpuNum].cpu_aft_usagetimeAll - CpuUsage.cpu[countCpuNum].cpu_pri_usagetimeAll));
        CpuUsage.cpu[countCpuNum].cpu_pri_usagetimeAll = 0;
        CpuUsage.cpu[countCpuNum].cpu_aft_usagetimeAll = 0;
        
        
    }


    printf("cpu1:%d cpu2:%d cpu3:%d cpu4:%d \n",CpuUsage.cpu[0].cpu_usageRate,CpuUsage.cpu[1].cpu_usageRate,CpuUsage.cpu[2].cpu_usageRate,CpuUsage.cpu[3].cpu_usageRate);

}
