#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

int main(){
    while(1){
        FILE *pf1=NULL;
        if( ( pf1=popen("cat /proc/stat","r") )==NULL ){
            return 0;
        }

        char str[768]={0};
        int i;
        int totalCpu1=0;
        int idle1=0;
        double usage;
        while(!feof(pf1)){
            fgets(str,768,pf1);
            char *takeCpu=strstr(str,"cpu ");
            char *token=strtok(takeCpu," ");
            char *data[11];
            int dataCount=0;
            int dataValue[11]={0};
            if(takeCpu != NULL){
                while(token != NULL){
                    data[dataCount]=token;
                    dataCount++;
                    token=strtok(NULL," ");
                }
                for(i=0;i<11;i++){
                    totalCpu1+=atoi(data[i]);
                    if(i==4){
                        idle1=atoi(data[i]);
                    }
                }
            }
        }
        fclose(pf1);

        usleep(100000);

        FILE *pf2=NULL;
        if( ( pf2=popen("cat /proc/stat","r") )==NULL ){
            return 0;
        }

        int totalCpu2=0;
        int idle2=0;
        while(!feof(pf1)){
            fgets(str,768,pf1);
            char *takeCpu=strstr(str,"cpu ");
            char *token=strtok(takeCpu," ");
            char *data[11];
            int dataCount=0;
            int dataValue[11]={0};
            if(takeCpu != NULL){
                while(token != NULL){
                    data[dataCount]=token;
                    dataCount++;
                    token=strtok(NULL," ");
                }
                for(i=0;i<11;i++){
                    totalCpu2+=atoi(data[i]);
                    if(i==4){
                        idle2=atoi(data[i]);
                    }
                }
            }
        }
        
        fclose(pf2);

        double totalCpu=totalCpu2-totalCpu1;
        double idle=idle2-idle1;
        usage=(totalCpu-idle)*100/totalCpu;
        printf("CPU Usage: %.02lf%%\n",usage);

        usleep(900000);
    }
}