#include<stdio.h>
#include<stdint.h>
#include"h02.h"

int main(){
        int32_t num;
        printf("Please enter the disk number (2-20):");
        scanf("%d",&num);
        if(num%2==1){
                oddDisk(num);
        }
        else{
                evenDisk(num);
        }
}