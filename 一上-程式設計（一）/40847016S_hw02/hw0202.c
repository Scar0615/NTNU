#include <stdio.h>
#include <stdint.h>

int main(){
    int32_t fare=0,dis,sec,rdis;
    int32_t nt,cny;
    printf("Please enter the taxi driving distance (meter):");
    scanf("%d", &dis);
    rdis=dis-1250;
    if(dis>=1250){
        fare+=70+(rdis/250)*5;
    }
    else{
        fare+=70;
    }
    
    printf("Please enter the taxi stoping time (second):");
    scanf("%d", &sec);
    if(sec>=100){
        fare+=5*(sec/100);
    }
    else{
        fare+=0;
    }
    
    printf("Is nighttime or not?(0:No, 1:Yes)");
    scanf("%d", &nt);
    if(nt==1){
        fare+=20;
    }
    else{
        fare+=0;
    }
    printf("Is Chinese New Year or not?(0:No, 1:Yes)");
    scanf("%d", &cny);
    if(cny==1){
        fare+=20;
    }
    else{
        fare+=0;
    }
    printf("Taxi fare: %d\n", fare);
}
