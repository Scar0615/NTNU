#include<stdio.h>
#include<string.h>

int main(){
    FILE *pf1=NULL;
    FILE *pf2=NULL;
    char filename[32]={0};
    char option[32]={0};
    char value[64]={0};
    char str[200]={0};
    char *word;
    char *store[2];
    int a=0,b=0,c=0;
    int i,cut,l,action=0;

    printf("Please enter the configuration file: ");

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
    if( (pf2=fopen("save.txt","w") )==NULL){
        printf("File could not be opened!\n");
        perror("Error");
        return 0;
    }

    printf("Option: ");

    if(fgets(option,sizeof(option),stdin)==NULL){
        printf("Error Input!\n");
        return 0;
    }
    if(option[strlen(option)-1]=='\n'){
        option[strlen(option)-1]=0;
    }
    else{
        while( (b=fgetc(stdin) )!='\n' && b!=EOF ){}
    }

    printf("Value: ");

    if(fgets(value,sizeof(value),stdin)==NULL){
        printf("Error Input!\n");
        return 0;
    }
    if(value[strlen(value)-1]=='\n'){
        value[strlen(value)-1]=0;
    }
    else{
        while( (c=fgetc(stdin) )!='\n' && c!=EOF ){}
    }

    while( fgets(str,sizeof(str),pf1) != NULL ){
        cut=0;l=0;
        if(str[0]>=97&&str[0]<=122){
            for(i=0;i<strlen(str);i++){
                if(str[i]=='='){
                    cut=1;
                }
            }
            if(cut==1){
                word=strtok(str,"=");
                while(word!=NULL){
                    store[l]=word;
                    word=strtok(NULL,"=");
                    l++;
                }
                if(strcmp(store[0],option)==0){
                    fprintf(pf2,"%s=%s\n",store[0],value);
                    action=1;
                }
                else{
                    fprintf(pf2,"%s=%s",store[0],store[1]);
                }
            }
            else{
                fprintf(pf2,"%s",str);
            }
        }
        else{
            fprintf(pf2,"%s",str);
        }
    }
    if(action==0){
        fprintf(pf2,"%s=%s\n",option,value);
    }

    printf("Done!\n");
    fclose(pf1);
    fclose(pf2);

    remove(filename);
    rename("save.txt", filename);
    return 0;
}