#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"test.h"

int main(){
    char filename[32]={0};
    char make[10]={0};
    char aout[10]={0};
    char read[50]={0};
    char *list="(), ;";
    char *r;
    char *store[5];
    int a=0,c,i,tc=0,value[50]={0},state[50]={0},series[50]={0}; //tc is test case
    FILE *pf1=NULL;

    printf("Please enter the test script:");
    if(fgets(filename,sizeof(filename),stdin)==NULL){
        printf("Error Input!\n");
        return 0;
    }
    if(filename[strlen(filename)-1]=='\n'){
        filename[strlen(filename)-1]=0;
    }
    else{
        while( (a=fgetc(stdin) )!='\n' && a!=EOF ){}
    }

    if( (pf1=fopen(filename,"r") )==NULL){
        printf("File could not be opened!\n");
        perror("Error");
        return 0;
    }

    while( fgets(read,sizeof(read),pf1) != NULL){
        r=strtok(read,list);
        c=0;
        while(r!=NULL){
            store[c]=r;
            r=strtok(NULL,list);
            c++;
        }
        if(strcmp(store[0],"EXPECT_EQ")==0){
            value[tc]=atoi(store[1]);
            if(strcmp(store[2],"fibonacci")==0){
                state[tc]=1;
            }
            else if(strcmp(store[2],"factorial")==0){
                state[tc]=2;
            }
            series[tc]=atoi(store[3]);
            tc++;
        }
    }

    printf("a.c and Makefile are generated.\n");
    printf("$ ");
    if(fgets(make,sizeof(make),stdin)==NULL){
        printf("Error Input!\n");
        perror("Error");
        return 0;
    }
    printf("$ ");
    if(fgets(aout,sizeof(aout),stdin)==NULL){
        printf("Error Input!\n");
        perror("Error");
        return 0;
    }

    printf("Running %d test cases:\n",tc);
    for(i=0;i<tc;i++){
        if(state[i]==1){
            if(value[i]==fibonacci(series[i])){
                printf("%d) fibonacci(%d): PASS\n",i+1,series[i]);
            }
            else{
                printf("%d) fibonacci(%d): %d(your value: %d)\n",i+1,series[i],fibonacci(series[i]),value[i]);
            }
        }
        else{
            if(value[i]==factorial(series[i])){
                printf("%d) factorial(%d): PASS\n",i+1,series[i]);
            }
            else{
                printf("%d) factorial(%d): %d(your value: %d)\n",i+1,series[i],factorial(series[i]),value[i]);
            }
        }
    }
    fclose(pf1);
}
