#include "getMemoryInfo.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char* MemInfoPath = "/proc/meminfo";

typedef struct mem_a
{
    int mem_total;
    int mem_free;
    int mem_buffer;
    int mem_cached;
    int mem_usage;
    char trash[100];
}mem_all;

mem_all MemUsage;

int getMemUsage()
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

        printf("Open Mem info file succ\n");
        fscanf(fp,"%s %d %s",&MemUsage.trash,&MemUsage.mem_total,&MemUsage.trash);
        fscanf(fp,"%s %d %s",&MemUsage.trash,&MemUsage.mem_free,&MemUsage.trash);
        fscanf(fp,"%s %d %s",&MemUsage.trash,&MemUsage.mem_buffer,&MemUsage.trash);
        fscanf(fp,"%s %d %s",&MemUsage.trash,&MemUsage.mem_buffer,&MemUsage.trash);
        fscanf(fp,"%s %d %s",&MemUsage.trash,&MemUsage.mem_cached,&MemUsage.trash);
        MemUsage.mem_usage = 100 - (100*1.0*(MemUsage.mem_free+MemUsage.mem_buffer+MemUsage.mem_cached)/MemUsage.mem_total);
        printf("total:%d free:%d buffers:%d cached:%d usagerate:%d\n",MemUsage.mem_total,MemUsage.mem_free,MemUsage.mem_buffer,MemUsage.mem_cached,MemUsage.mem_usage);
        fclose(fp);
        return MemUsage.mem_usage;
    }

}