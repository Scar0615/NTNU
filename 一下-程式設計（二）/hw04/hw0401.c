#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    FILE *pf=NULL;
    if( ( pf=fopen(argv[argc-1],"r") )==NULL ){
        printf("File could not be opened!\n");
        return 0;
    }
    //處理-F
    int oF=0; //oF = option F | 0 = false | 1 = true
    char *sep=" "; //sep = separator
    int c=0;
    int index=0;
    int i,j;
    while( ( c=getopt(argc,argv,"F") )!=-1 ){
        switch(c){
            case 'F':
            oF=1;
            break;
            default:
            break;
        }
    }
    if(oF==1){
        sep=argv[2];
    }
    //取出'$x'裡的x
    argv[argc-4][0]='0';
    argv[argc-2][0]='0';
    int loc1=atoi(argv[argc-4]); //loc = location
    int loc2=atoi(argv[argc-2]);
    //檔案處理
    char str[200]={0};
    while(!feof(pf)){
        fgets(str,sizeof(str),pf);
        if(str[strlen(str)-1]=='\n'){
            str[strlen(str)-1]=0;
        }
        char *take;
        char *sec[20]; //sec = section
        int count=0; //a counter for section
        take=strtok(str,sep);
        while(take!=NULL){
            sec[count]=take;
            take=strtok(NULL,sep);
            count++;
        }
        printf("%s%s%s\n",sec[loc1-1],argv[argc-3],sec[loc2-1]);
        for(i=0;i<count;i++){
            for(j=0;j<strlen(sec[i]);j++){
                sec[i][j]=0;
            }
        }
    }

    fclose(pf);
    return 0;
}