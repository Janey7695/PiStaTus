#include "./Oled_driver/oled.h"
#include "./Oled_driver/draw_api.h"
#include "./Oled_driver/img.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <string.h>

#define Version "1.0.0"
#define PriTest 1
#define AftTest 0
#define UsageRectBeigin_x 1
#define UsageRectBeigin_y 35
#define UsageRectWidth 5
#define UsageRectLength 80
#define UsageRectInter 2
#define LineChartLength 40
#define LineChartWidth 27
#define LineChartBegin_x 85
#define LineChartBegin_y 35
#define MemLineChartLength 40
#define MemLineChartWidth 27
#define MemLineChartBegin_x 85
#define MemLineChartBegin_y 0

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
    cpu_usage cpu[5];
}cpu_all;

typedef struct mem_a
{
    int mem_total;
    int mem_free;
    int mem_buffer;
    int mem_cached;
    int mem_usage;
    char trash[100];
}mem_all;

cpu_usage cpu_1,cpu_2,cpu_3,cpu_4,cpu_Al;
cpu_all CpuUsage;
mem_all MemUsage;
char temp[4];
char cpuAllUsageLineChartDat[LineChartLength];
char memAllUsageLineChartDat[MemLineChartLength];
unsigned char IpnetText[104];
unsigned char IpnetToShow[32];
char* TempFilePathCmd = "cat /sys/class/thermal/thermal_zone0/temp";
char* CpuInfoPath = "/proc/stat";
char* MemInfoPath = "/proc/meminfo";
char* IpCmd = "ip a";

void GetTemp(void);
void GetCpuUasge(void);
void GetMemUsage(void);
void IpAddressInit(void);
void GetIpAddress(void);


void Draw_UI()
{
    OledPaint.Draw.Rect(MemLineChartBegin_x,MemLineChartBegin_y,MemLineChartBegin_x+MemLineChartLength,MemLineChartBegin_y+MemLineChartWidth,0);
    OledPaint.Draw.Rect(LineChartBegin_x,LineChartBegin_y,LineChartBegin_x+LineChartLength,LineChartBegin_y+LineChartWidth,0);
    OledPaint.Draw.Picture(0,0,38,28,cpu_temp_img);
    for(int i=0;i<4;i++)
    {
        OledPaint.Draw.Rect(UsageRectBeigin_x,UsageRectBeigin_y+(UsageRectWidth+UsageRectInter)*i,UsageRectBeigin_x+UsageRectLength,UsageRectWidth+UsageRectBeigin_y+(UsageRectWidth+UsageRectInter)*i,0);
    }
}

void updateLineCHartDat(cpu_all* CpuUsAge)
{
    float usgRate=0.0;
    int usgRateLen =0;
    usgRate = CpuUsAge->cpu[0].cpu_usageRate/100.0;
    for(int count =0;count<LineChartLength-1;count++)
    {
        cpuAllUsageLineChartDat[count] = cpuAllUsageLineChartDat[count+1];
    }
    cpuAllUsageLineChartDat[LineChartLength-1] = usgRate*LineChartWidth;
}

void updateMemLineCHartDat(mem_all* MemUsAge)
{
    float usgRate=0.0;
    int usgRateLen =0;
    usgRate = MemUsAge->mem_usage/100.0;
    for(int count =0;count<MemLineChartLength-1;count++)
    {
        memAllUsageLineChartDat[count] = memAllUsageLineChartDat[count+1];
    }
    memAllUsageLineChartDat[MemLineChartLength-1] = usgRate*MemLineChartWidth;
}

void Draw_fillInfo(char *temp,cpu_all* CpuUsAge)
{
    float usgRate=0.0;
    int usgRateLen =0;
    OledPaint.Show.Str(42,0,temp,16);
    OledPaint.Draw.Picture(42,8,32,8,IpnetToShow);
    for(int countCpuNum=0;countCpuNum<4;countCpuNum++)
    {
        usgRate = CpuUsAge->cpu[countCpuNum+1].cpu_usageRate/100.0;
        usgRateLen = usgRate * UsageRectLength;
        OledPaint.Draw.Rect(UsageRectBeigin_x,UsageRectBeigin_y+(UsageRectWidth+UsageRectInter)*countCpuNum,UsageRectBeigin_x+ usgRateLen,UsageRectWidth+UsageRectBeigin_y+(UsageRectWidth+UsageRectInter)*countCpuNum,1);
    }
    updateLineCHartDat(&CpuUsage);
    for(int countPointNumb =0;countPointNumb<LineChartLength-1;countPointNumb++)
    {
        OledPaint.Draw.Line(countPointNumb+LineChartBegin_x,LineChartBegin_y+LineChartWidth-cpuAllUsageLineChartDat[countPointNumb],countPointNumb+LineChartBegin_x+1,LineChartBegin_y+LineChartWidth-cpuAllUsageLineChartDat[countPointNumb+1]);
    }
    updateMemLineCHartDat(&MemUsage);
    for(int countPointNumb =0;countPointNumb<LineChartLength-1;countPointNumb++)
    {
        OledPaint.Draw.Line(countPointNumb+MemLineChartBegin_x,MemLineChartBegin_y+MemLineChartWidth-memAllUsageLineChartDat[countPointNumb],countPointNumb+MemLineChartBegin_x+1,MemLineChartBegin_y+MemLineChartWidth-memAllUsageLineChartDat[countPointNumb+1]);
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
    IpAddressInit();
    CpuUsage.cpu[1] = cpu_1;
    CpuUsage.cpu[2] = cpu_2;
    CpuUsage.cpu[3] = cpu_3;
    CpuUsage.cpu[4] = cpu_4;
    CpuUsage.cpu[0] = cpu_Al;
    memset(&MemUsage,0,sizeof(MemUsage));
    memset(&cpuAllUsageLineChartDat,0,sizeof(cpuAllUsageLineChartDat));
    memset(&memAllUsageLineChartDat,0,sizeof(memAllUsageLineChartDat));
    while(1)
    {
        
        CanvasClear();
        Draw_UI();
        GetTemp();
        GetCpuUasge();
        GetMemUsage();
        Draw_fillInfo(temp,&CpuUsage);
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
        float temp2;
        int temp1;
        fgets(buffer,sizeof(buffer),fp);
        temp1 = atoi(buffer);
        temp2 = temp1/1000.0;
        sprintf(temp,"%.1f",temp2);
        printf("now tempture is %s 'C\n",temp);
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
        if(Ifpri)
        {
            for(int countCpuNum=0;countCpuNum<5;countCpuNum++)
            {
                fscanf(fp,"%s %d %d %d %d %d %d %d",&CpuUsage.cpu[countCpuNum].cpuname,&CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[0],&CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[1],&CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[2],&CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[3],&CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[4],&CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[5],&CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[6]);
                printf("%d %d %d %d %d %d %d\n",CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[0],CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[1],CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[2],CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[3],CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[4],CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[5],CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[6]);
                fgets(buffer,sizeof(buffer),fp);
            }
            printf("read all pri_cpuinfo .. \n");
        }
        else
        {
            for(int countCpuNum=0;countCpuNum<5;countCpuNum++)
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
    memset(&cpu_Al,0,sizeof(cpu_Al));
    GetInfoFromFile(PriTest);
    delay(100);
    GetInfoFromFile(AftTest);
    for(countCpuNum=0;countCpuNum<5;countCpuNum++)
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


    printf("cpu:%d cpu1:%d cpu2:%d cpu3:%d cpu4:%d \n",CpuUsage.cpu[0].cpu_usageRate,CpuUsage.cpu[1].cpu_usageRate,CpuUsage.cpu[2].cpu_usageRate,CpuUsage.cpu[3].cpu_usageRate,CpuUsage.cpu[4].cpu_usageRate);

}

void GetMemUsage()
{
    FILE *fp;
    char buffer[100];
    fp = fopen(MemInfoPath,"r");
    if(fp == NULL)
    {
        printf("Open Mem info file fail\n");
        exit(1);
    }
    else
    {

        printf("Open Cpu info file succ\n");
        fscanf(fp,"%s %d %s",&MemUsage.trash,&MemUsage.mem_total,&MemUsage.trash);
        fscanf(fp,"%s %d %s",&MemUsage.trash,&MemUsage.mem_free,&MemUsage.trash);
        fscanf(fp,"%s %d %s",&MemUsage.trash,&MemUsage.mem_buffer,&MemUsage.trash);
        fscanf(fp,"%s %d %s",&MemUsage.trash,&MemUsage.mem_buffer,&MemUsage.trash);
        fscanf(fp,"%s %d %s",&MemUsage.trash,&MemUsage.mem_cached,&MemUsage.trash);
        MemUsage.mem_usage = 100 - (100*1.0*(MemUsage.mem_free+MemUsage.mem_buffer+MemUsage.mem_cached)/MemUsage.mem_total);
        printf("total:%d free:%d buffers:%d cached:%d usagerate:%d\n",MemUsage.mem_total,MemUsage.mem_free,MemUsage.mem_buffer,MemUsage.mem_cached,MemUsage.mem_usage);

    }

    fclose(fp);
}

void IpAddressInit()
{
    FILE *fp;
    char buffer[200],ipAddress[20];
    fp = popen(IpCmd,"r");
    for(int i=0;i<10;i++)
    {
        fgets(buffer,sizeof(buffer),fp);
    }
    fscanf(fp,"%s %s",&buffer,&ipAddress);
    printf("ip address is:%s\n",ipAddress);
    pclose(fp);
}
