#include<stdio.h>  
#include<stdlib.h>  
#include<stdint.h>
#include<string.h>
#include<unistd.h>

//這隻程式最多可容納5張網卡

void bps(int x){
    if(x>=1000000){
        printf("%dMbps",x/1000000);
    }
    else if(x>=1000){
        printf("%dKbps",x/1000);
    }
    else{
        printf("%dbps",x);
    }
}

int main(){
    int time=1;
    while(1){
        //開頭順序
        printf("%d)\n",time);
        time++;
        //正事開始
        FILE *pf1=NULL;
        if( ( pf1=popen("ifconfig","r") )==NULL ){
            return 0;
        }
        char buffer[256];
        int lcount=0; //l=lines
        int bcount=0;
        int i,j; //迴圈變數放這
        int by[10];
        while(!feof(pf1)){
            fgets(buffer,sizeof(buffer),pf1);
            //取出名字
            lcount++;
            if(strlen(buffer)==1){
                lcount=0;
            }
            if(lcount==1){
                for(i=0;i<strlen(buffer);i++){
                    if(buffer[i]==':'){
                        j=i;
                        break;
                    }
                }
                for(i=0;i<j;i++){
                    printf("%c",buffer[i]);
                }
            }
            //取出ip位址
            char *take=strstr(buffer,"inet ");
            if(take!=NULL){
                char *f=strtok(take," "); //f=fraction
                char *rf[6]; //r=record
                int fcount=0;
                int ncount=0;
                while(f!=NULL){
                    rf[fcount]=f;
                    f=strtok(NULL," ");
                    fcount++;
                }
                printf(" (%s): ",rf[1]);
            }
            //取出RX/TX
            char *tb=strstr(buffer,"bytes"); //take byte
            if(tb!=NULL){
                char *sd=strtok(tb," "); //separate data
                char *rd[4]; 
                int rcount=0;
                while(sd!=NULL){
                    rd[rcount]=sd;
                    sd=strtok(NULL," ");
                    rcount++;
                }
                by[bcount]=atoi(rd[1])*8;
                bcount++;
            }
            if(bcount%2==0 && tb!=NULL){
                printf("TX: ");
                bps(by[bcount-1]);
                printf("; RX: ");
                bps(by[bcount-2]);
                printf("\n");
            }
        }

        pclose(pf1);
        sleep(1);
    }
}