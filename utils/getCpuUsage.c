#include "getCpuUsage.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wiringPi.h>

char* CpuInfoPath = "/proc/stat";

unsigned int lastUpdateTime = 0;
int calculate_flag = 0;
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

// cpu_usage cpu_1,cpu_2,cpu_3,cpu_4,cpu_Al;
cpu_all CpuUsage;


void getCpuStatus()
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
        for(int countCpuNum=0;countCpuNum<5;countCpuNum++)
        {
            fscanf(fp,"%s %d %d %d %d %d %d %d",CpuUsage.cpu[countCpuNum].cpuname,&CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[0],&CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[1],&CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[2],&CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[3],&CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[4],&CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[5],&CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[6]);
            printf("%d %d %d %d %d %d %d\n",CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[0],CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[1],CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[2],CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[3],CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[4],CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[5],CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[6]);
            fgets(buffer,sizeof(buffer),fp);
        }
    }
    fclose(fp);
}

void setPri2Aft(){
    int count=0;
    int countCpuNum =0;
    for(countCpuNum=0;countCpuNum<5;countCpuNum++)
        {
            for(count=0;count<=7;count++)
            {
                CpuUsage.cpu[countCpuNum].cpu_atf_usagetime[count] = CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[count];
                //CpuUsage.cpu[countCpuNum].cpu_aft_usagetimeAll = CpuUsage.cpu[countCpuNum].cpu_aft_usagetimeAll + CpuUsage.cpu[countCpuNum].cpu_atf_usagetime[count];
            }
            // CpuUsage.cpu[countCpuNum].cpu_usageRate = 100 - (100*1.0*(CpuUsage.cpu[countCpuNum].cpu_atf_usagetime[3]-CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[3])/(CpuUsage.cpu[countCpuNum].cpu_aft_usagetimeAll - CpuUsage.cpu[countCpuNum].cpu_pri_usagetimeAll));
            // CpuUsage.cpu[countCpuNum].cpu_pri_usagetimeAll = 0;
            // CpuUsage.cpu[countCpuNum].cpu_aft_usagetimeAll = 0;
        }
}

void calculateCpuUasge(unsigned int updateInterval)
{   
    int count=0;
    int countCpuNum =0;
    if(millis() - lastUpdateTime <= updateInterval){
        return ;
    }
    updateInterval = millis();
    //printf("begin to clear cpuinfo\n");
    //memset(&CpuUsage,0,sizeof(cpu_all));
    // memset(&cpu_2,0,sizeof(cpu_2));
    // memset(&cpu_3,0,sizeof(cpu_3));
    // memset(&cpu_4,0,sizeof(cpu_4));
    // memset(&cpu_Al,0,sizeof(cpu_Al));
    getCpuStatus();
    if(calculate_flag == 0){
        calculate_flag = 1;
        return ;
    }
    if(calculate_flag){
        setPri2Aft();
        getCpuStatus();

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
        calculate_flag = 0;
    }
    // delay(100);
    // getCpuStatus();
    // for(countCpuNum=0;countCpuNum<5;countCpuNum++)
    // {
    //     for(count=0;count<=6;count++)
    //     {
    //         CpuUsage.cpu[countCpuNum].cpu_pri_usagetimeAll = CpuUsage.cpu[countCpuNum].cpu_pri_usagetimeAll + CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[count];
    //         CpuUsage.cpu[countCpuNum].cpu_aft_usagetimeAll = CpuUsage.cpu[countCpuNum].cpu_aft_usagetimeAll + CpuUsage.cpu[countCpuNum].cpu_atf_usagetime[count];

    //     }
    //     CpuUsage.cpu[countCpuNum].cpu_usageRate = 100 - (100*1.0*(CpuUsage.cpu[countCpuNum].cpu_atf_usagetime[3]-CpuUsage.cpu[countCpuNum].cpu_pri_usagetime[3])/(CpuUsage.cpu[countCpuNum].cpu_aft_usagetimeAll - CpuUsage.cpu[countCpuNum].cpu_pri_usagetimeAll));
    //     CpuUsage.cpu[countCpuNum].cpu_pri_usagetimeAll = 0;
    //     CpuUsage.cpu[countCpuNum].cpu_aft_usagetimeAll = 0;
    // }
    // printf("cpu:%d cpu1:%d cpu2:%d cpu3:%d cpu4:%d \n",CpuUsage.cpu[0].cpu_usageRate,CpuUsage.cpu[1].cpu_usageRate,CpuUsage.cpu[2].cpu_usageRate,CpuUsage.cpu[3].cpu_usageRate,CpuUsage.cpu[4].cpu_usageRate);

}

int getCpuAverageUsage(){
    return CpuUsage.cpu[0].cpu_usageRate;
}

int getCpuXUsage(int x){
    if(x > 3){
        return 0;
    }
    return CpuUsage.cpu[x+1].cpu_usageRate;
}