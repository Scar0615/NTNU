#include <stdio.h>
#include <stdint.h>

int main(){
    int32_t num[5],state=0,order=0;
    printf("Please enter 5 integers:");
    for(int32_t i=0;i<5;i++){
        scanf("%d", &num[i]);
        switch(state){
            case 0:
                if(num[i]==10 || num[i]==35){
                    state=1;
                    order++;
                }
                else if(num[i]==11){
                    state=3;
                    order++;
                }
                else if(num[i]==20 || num[i]==78){
                    state=5;
                    order++;
                }
                else{
                    state=0;
                    order++;
                }
                break;
            case 1:
                if(num[i]==19){
                    state=2;
                    order++;
                }
                else if(num[i]==12 || num[i]==36){
                    state=6;
                    order++;
                }
                else{
                    state=1;
                    order++;
                }        
                break;
            case 2:
                if(num[i]==43){
                    state=2;
                    order++;
                }
                else if(num[i]==99){
                    state=7;
                    order++;
                }
                else{
                    state=0;
                    order++;
                }        
                break;
            case 3:
                state=4;
                order++;
                break;
            case 4:
                state=6;
                order++;
                break;
            case 5:
                if(num[i]==1){
                    state=4;
                    order++;
                }
                else if(num[i]==2){
                    state=6;
                    order++;
                }
                else{
                    state=0;
                    order++;
                }        
                break;
            case 6:
                if(num[i]==108){
                    state=7;
                    order++;
                }
                else{
                    state=5;
                    order++;
                }        
                break;
            case 7:
                order++;
            	break;
        }
    }
    
    if(order==5){
        if(state==0){
            printf("The user is in the start state.\n");
        }
        else if(state==7){
            printf("The user is in the final state.\n");
        }
        else{
            printf("The user is in S%d.\n", state);
        }
    }
}
