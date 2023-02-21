#include<stdio.h>
#include<string.h>

int main(){
    char str[100];
    char *token;
    char p[6][2];
    int c=0,i,j;
    printf("Please enter the string:");
    fgets(str,100,stdin);
    token=strtok(str,":");
    while(token!=NULL){
        if(c==5){
            if(strlen(token)!=3){
                printf("invalid\n");
                return 0;
            }
        }
        else{
            if(strlen(token)!=2){
                printf("invalid\n");
                return 0;
            }
        }
        for(i=0;i<2;i++){
            if((token[i]>=48&&token[i]<=57)||(token[i]>=65&&token[i]<=70)||(token[i]>=97&&token[i]<=102)){
                p[c][i]=token[i];
            }
            else{
                printf("invalid\n");
                return 0;
            }
        }
        token = strtok(NULL,":");
        c++;
    }
    if(c>6){
        printf("invalid\n");
    }
    else{
        for(i=0;i<6;i++){
            for(j=0;j<2;j++){
                printf("%c",p[i][j]);
            }
            if(i!=5){
                printf("-");
            }
        }
        return 0;
    }
}