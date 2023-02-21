#include<stdio.h>
#include<stdint.h>
#include<string.h>

int main(){
    FILE *pf1=NULL;
    //輸入檔名
    char filename[50]={0};
    printf("Please enter the file:");
    if(fgets(filename,sizeof(filename),stdin)==NULL){
        printf("Error Input!\n");
        return 0;
    }
    if(filename[strlen(filename)-1]=='\n'){
        filename[strlen(filename)-1]=0;
    }
    else{
        int a;
        while( (a=fgetc(stdin) )!='\n' && a!=EOF ){}
    }
    if( ( pf1=fopen(filename,"rb") )==NULL ){
        printf("File1 could not be opened!\n");
        return 0;
    }
    //將資料印出來
    int count=0;
    while(!feof(pf1)){
        uint16_t data;
        fread(&data,2,1,pf1);
        if(count%8==0){
            printf("%08x %x",count*2,data);
        }
        else if(count%8==7){
            printf(" %x\n",data);
        }
        else{
            printf(" %x",data);
        }
        count++;
    }
    printf("\n00%x\n",count*2-2);
}