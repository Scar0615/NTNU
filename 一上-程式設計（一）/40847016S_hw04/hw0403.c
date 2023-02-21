#include<stdio.h>
#include<stdint.h>

int main(){
        int32_t memory[10],act,i,count=0,pos=0;
        printf("Please enter your action:");
        scanf("%d",&act);
        while(act!=0){
                if(act>0){
                        memory[count]=act;
                        count++;
                        pos++;
                }
                printf("Please enter your action:");
                scanf("%d",&act);
                if(act==-1){
                        pos--;
                }
                if(act==-2){
                        pos++;
                        memory[count]=0;
                        count--;
                }
        }
        for(i=0;i<count;i++){
                printf("%d",memory[i]);
                if(i==pos-1){
                        printf("(*)");
                }
                printf(" ");
        }
        printf("\n");
}