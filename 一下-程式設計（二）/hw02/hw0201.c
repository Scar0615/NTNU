#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){
    int motion;
    char inputf[32]={0};
    char outputf[32]={0};
    char keyf[32]={0};
    char str[200]={0};
    char trans[200]={0};
    char checkk[10]={0};
    char upper[26][2];
    char lower[26][2];
    FILE *pf1=NULL; //input file
    FILE *pf2=NULL; //output file
    FILE *pf3=NULL; //key file
    int i=0,o=0,k=0,t=0,u=0;

    printf("Please enter the input file:");
    if(fgets(inputf,sizeof(inputf),stdin)==NULL){
        printf("Error Input!\n");
        return 0;
    }
    if(inputf[strlen(inputf)-1]=='\n'){
        inputf[strlen(inputf)-1]=0;
    }
    else{
        while( (i=fgetc(stdin) )!='\n' && i!=EOF ){}
    }
    if( (pf1=fopen(inputf,"r") )==NULL){
        printf("File could not be opened!\n");
        perror("Error");
        return 0;
    }

    printf("Please enter the output file:");
    if(fgets(outputf,sizeof(outputf),stdin)==NULL){
        printf("Error Input!\n");
        return 0;
    }
    if(outputf[strlen(outputf)-1]=='\n'){
        outputf[strlen(outputf)-1]=0;
    }
    else{
        while( (o=fgetc(stdin) )!='\n' && o!=EOF ){}
    }
    if( (pf2=fopen(outputf,"w") )==NULL){
        printf("File could not be opened!\n");
        perror("Error");
        return 0;
    }

    printf("Please enter the key file:");
    if(fgets(keyf,sizeof(keyf),stdin)==NULL){
        printf("Error Input!\n");
        return 0;
    }
    if(keyf[strlen(keyf)-1]=='\n'){
        keyf[strlen(keyf)-1]=0;
    }
    else{
        while( (k=fgetc(stdin) )!='\n' && k!=EOF ){}
    }
    if( (pf3=fopen(keyf,"r") )==NULL){
        printf("File could not be opened!\n");
        perror("Error");
        return 0;
    }
    while(!feof(pf3)){
        fgets(checkk,10,pf3);
        u=0;
        for(int i=0;i<strlen(checkk);i++){
            if(checkk[i]>=65&&checkk[i]<=90){
                upper[t][u]=checkk[i];
                lower[t][u]=checkk[i]+32;
                u++;
            }
        }
        t++;
        for(int i=0;i<strlen(checkk);i++){checkk[i]=0;}
    }

    printf("Encryption (1) or Decryption (2):");
    scanf("%d",&motion);
    if(motion==1){
        while(!feof(pf1)){
            if(fgets(str,200,pf1)==NULL){
                break;
            }
            if(str[strlen(str)-1]=='\n'){
                str[strlen(str)-1]=0;
            }
            else{
                int c=0;
                while( ( c = fgetc( stdin ) ) != '\n' && c != EOF ){}
            }
            for(int i=0;i<strlen(str);i++){
                if(str[i]>=65&&str[i]<=90){
                    for(int j=0;j<26;j++){
                        if(str[i]==upper[j][0]){
                            trans[i]=upper[j][1];
                        }
                    }
                }
                else if(str[i]>=97&&str[i]<=122){
                    for(int j=0;j<26;j++){
                        if(str[i]==lower[j][0]){
                            trans[i]=lower[j][1];
                        }
                    }
                }
                else{
                    trans[i]=str[i];
                }
            }
            fprintf(pf2,"%s\n",trans);
        }
    }
    else if(motion==2){
        while(!feof(pf1)){
            if(fgets(str,200,pf1)==NULL){
                break;
            }
            if(str[strlen(str)-1]=='\n'){
                str[strlen(str)-1]=0;
            }
            else{
                int c=0;
                while( ( c = fgetc( stdin ) ) != '\n' && c != EOF ){}
            }
            for(int i=0;i<strlen(str);i++){
                if(str[i]>=65&&str[i]<=90){
                    for(int j=0;j<26;j++){
                        if(str[i]==upper[j][1]){
                            trans[i]=upper[j][0];
                        }
                    }
                }
                else if(str[i]>=97&&str[i]<=122){
                    for(int j=0;j<26;j++){
                        if(str[i]==lower[j][1]){
                            trans[i]=lower[j][0];
                        }
                    }
                }
                else{
                    trans[i]=str[i];
                }
            }
            fprintf(pf2,"%s\n",trans);
        }
    }

    fclose(pf1);
    fclose(pf2);
    fclose(pf3);

    printf("Done!\n");
}
