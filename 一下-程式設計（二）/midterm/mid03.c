#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>

int main(){
    FILE *pf1=NULL;
    FILE *pf2=NULL;

    if( ( pf1=fopen("","") )==NULL ){
        printf("File could not be opened!\n");
        return 0;
    }
    
    if( ( pf2=fopen("","") )==NULL ){
        printf("File could not be opened!\n");
        return 0;
    }
}