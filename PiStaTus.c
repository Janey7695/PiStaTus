#include "./Oled_driver/oled.h"
#include "./Oled_driver/draw_api.h"
#include "./Oled_driver/img.h"
#include "./utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <string.h>

#define Version "1.2.0"
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

char* temp;
char cpuAllUsageLineChartDat[LineChartLength];
char memAllUsageLineChartDat[MemLineChartLength];
unsigned char IpnetText[120*2];
unsigned char IpnetToShow[64];
int jindutiao = 0;
unsigned int lastTimeUpdateIp = 0;
unsigned int lastTimeUpdateTemp = 0;



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

void updateLineCHartDat(int CpuAverageUsage)
{
    float usgRate=0.0;
    int usgRateLen =0;
    usgRate = CpuAverageUsage/100.0;
    for(int count =0;count<LineChartLength-1;count++)
    {
        cpuAllUsageLineChartDat[count] = cpuAllUsageLineChartDat[count+1];
    }
    cpuAllUsageLineChartDat[LineChartLength-1] = usgRate*LineChartWidth;
}

void updateMemLineCHartDat(int MemUsage)
{
    float usgRate=0.0;
    int usgRateLen =0;
    usgRate = MemUsage/100.0;
    for(int count =0;count<MemLineChartLength-1;count++)
    {
        memAllUsageLineChartDat[count] = memAllUsageLineChartDat[count+1];
    }
    memAllUsageLineChartDat[MemLineChartLength-1] = usgRate*MemLineChartWidth;
}

void Draw_fillInfo()
{
    float usgRate=0.0;
    int usgRateLen =0;
    
    //填充温度与ip信息
    if(millis() - lastTimeUpdateIp >= 400){
        IPText_WriteString(IpnetText,getIp(),16);
        jindutiao+=8;
        if(jindutiao>=120)
        jindutiao=0;
        lastTimeUpdateIp = millis();
    }
    OledPaint.Show.Str(42,0,temp,16);
    Draw_PicPart(jindutiao,IpnetToShow,IpnetText);
    OledPaint.Draw.Picture(42,16,32,16,IpnetToShow);
        
    //绘制Cpu进度条
    for(int countCpuNum=0;countCpuNum<4;countCpuNum++)
    {
        usgRate = getCpuXUsage(countCpuNum)/100.0;
        usgRateLen = usgRate * UsageRectLength;
        OledPaint.Draw.Rect(UsageRectBeigin_x,UsageRectBeigin_y+(UsageRectWidth+UsageRectInter)*countCpuNum,UsageRectBeigin_x+ usgRateLen,UsageRectWidth+UsageRectBeigin_y+(UsageRectWidth+UsageRectInter)*countCpuNum,1);
    }
    //绘制Cpu利用率折线图
    for(int countPointNumb =0;countPointNumb<LineChartLength-1;countPointNumb++)
    {
        OledPaint.Draw.Line(countPointNumb+LineChartBegin_x,LineChartBegin_y+LineChartWidth-cpuAllUsageLineChartDat[countPointNumb],countPointNumb+LineChartBegin_x+1,LineChartBegin_y+LineChartWidth-cpuAllUsageLineChartDat[countPointNumb+1]);
    }
    //绘制内存利用率折线图
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
    memset(&cpuAllUsageLineChartDat,0,sizeof(cpuAllUsageLineChartDat));
    memset(&memAllUsageLineChartDat,0,sizeof(memAllUsageLineChartDat));
    while(1)
    {
        
        CanvasClear();
        Draw_UI();
        if(millis() - lastTimeUpdateTemp >= 200){
            temp = getTemp();
            lastTimeUpdateTemp = millis();
        }
        
        calculateCpuUasge(50);
        updateLineCHartDat(getCpuAverageUsage());
        updateMemLineCHartDat(getMemUsage());
        Draw_fillInfo();
        DisPlay();
        delay(50);
    }
}

