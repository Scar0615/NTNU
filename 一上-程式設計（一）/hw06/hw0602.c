#include<stdio.h>

int state_start(int *,int );
int state_s01(int *,int );
int state_s02(int *,int );
int state_s03(int *,int );
int state_s04(int *,int );
int state_s05(int *,int );
int state_s06(int *,int );
int state_final(int *,int );

int main(){
        int (*dfa[8])(int *,int );
        dfa[0]=state_start;
        dfa[1]=state_s01;
        dfa[2]=state_s02;
        dfa[3]=state_s03;
        dfa[4]=state_s04;
        dfa[5]=state_s05;
        dfa[6]=state_s06;
        dfa[7]=state_final;
        int input,state=0,tmp;
        while(1){
                printf("Please enter an integer (0-255, -1: stop):");
                scanf("%d",&input);
                if(input<0 || input>255 || input==-1){
                        if(state==0){
                                printf("The user is in the start state.\n");
                        }
                        else if(state==7){
                                printf("The user is in the final state.\n");
                        }
                        else{
                                printf("The user is in S%d.\n", state);
                        }
                        return 0;
                }
                else{
                        tmp=state;
                        (*dfa[tmp])(&state,input);
                }
        }
}

int state_start(int *state,int input){
        if(input==10 || input==35){
                *state=1;
        }
        else if(input==11){
                *state=3;
        }
        else if(input==20 || input==78){
                *state=5;
        }
        else{
                *state=0;
        }
        return *state;
}
int state_s01(int *state,int input){
        if(input==19){
                *state=2;
        }
        else if(input==12 || input==36){
                *state=6;
        }
        else{
                *state=1;
        }
        return *state;
}
int state_s02(int *state,int input){
        if(input==43){
                *state=2;
        }
        else if(input==99){
                *state=7;
        }
        else{
                *state=0;
        }
        return *state;
}
int state_s03(int *state,int input){
        if(input>=0 && input<=255){
                *state=4;
        }
        return *state;
}
int state_s04(int *state,int input){
        if(input>=0 && input<=255){
                *state=6;
        }
        return *state;
}
int state_s05(int *state,int input){
        if(input==1){
                *state=4;
        }
        else if(input==2){
                *state=6;
        }
        else{
                *state=0;
        }
        return *state;
}
int state_s06(int *state,int input){
        if(input==108){
                *state=7;
        }
        else{
                *state=5;
        }
        return *state;
}
int state_final(int *state,int input){
        if(input>=0 && input<=255){
                *state=7;
        }
        return *state;
}