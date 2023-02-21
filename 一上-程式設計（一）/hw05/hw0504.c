#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<unistd.h>

int main(){
        uint32_t msec;
        int32_t i,num[10];
        printf("Please enter the time interval (1-1000 msec):");
        scanf("%d",&msec);
        for(i=0;i<10;i++){
                num[i]=i;
        }
        while(1){
                for(i=0;i<10;i++){
                        if(num[i]==9){
                                num[i]=-1;
                        }
                        num[i]++;
                        printf("%d",num[i]);
                }
                usleep(msec*500);
                printf("\n");
                usleep(msec*500);
                system("clear");
        }
}