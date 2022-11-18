#include "getNetstatus.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char* IpCmd = "ip a";
char ip[20] = "127.0.0.1";


//定位至要查询ip的设备的下一行
void findDevice(const char* device,FILE *fp){
    char buffer[100],buffer2[100];
    fscanf(fp,"%s %s",buffer,buffer2);
    while(buffer2[0]!=device[0]){
        fgets(buffer,sizeof(buffer),fp);
        fscanf(fp,"%s %s",buffer,buffer2);
       // printf("%s \r\n",buffer2);
    }
}

//尝试获取设备的ip
int getIpAddress(const char* device,FILE *fp){
    char buffer[100],buffer2[100];
    findDevice(device,fp);
    fgets(buffer,sizeof(buffer),fp);
    fgets(buffer,sizeof(buffer),fp);
    fscanf(fp,"%s %s",buffer,buffer2);
    if(buffer2[0]<='9'){
        strcpy(ip,buffer2);
        return 1;
    }
    else return 0;
}

char* getIp()
{
    FILE *fp;
    char buffer[200],ipAddress[20];
    fp = popen(IpCmd,"r");
    if(getIpAddress("eth0",fp)){
        pclose(fp);
        return ip;
    }else if(getIpAddress("wlan0",fp)){
        pclose(fp);
        return ip;
    }else{
        pclose(fp);
        return "No Connect";
    }

}