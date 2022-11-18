#include "getTemperature.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char* TempFilePathCmd = "cat /sys/class/thermal/thermal_zone0/temp";
char utils_temp[4];
const char queryTempFail[5] = "fail";

char* getTemp(void)
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
        sprintf(utils_temp,"%.1f",temp2);
        pclose(fp);
        //printf("now tempture is %s 'C\n",temp);
        return utils_temp;
    }
    else
    {
        //printf("read tempture fail\n");
        pclose(fp);
        return queryTempFail;
    }
}