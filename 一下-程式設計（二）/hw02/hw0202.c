#include<stdio.h>
#include<stdlib.h>
#include<string.h>

FILE *pf1=NULL;
FILE *pf2=NULL;

void timePrint(int t){
    int i;
    char table[10]={0};
    sprintf(table,"%d",t);
    if(strlen(table)==5){
        fprintf(pf2,"00:00:");
        for(i=0;i<5;i++){
            if(i==1){
                fprintf(pf2,"%c,",table[i]);
            }
            else{
                fprintf(pf2,"%c",table[i]);
            }
        }
    }
    else if(strlen(table)==6){
        fprintf(pf2,"00:0");
        for(i=0;i<6;i++){
            if(i==0){
                fprintf(pf2,"%c:",table[i]);
            }
            else if(i==2){
                fprintf(pf2,"%c,",table[i]);
            }
            else{
                fprintf(pf2,"%c",table[i]);
            }
        }
    }
    else if(strlen(table)==7){
        fprintf(pf2,"00:");
        for(i=0;i<7;i++){
            if(i==1){
                fprintf(pf2,"%c:",table[i]);
            }
            else if(i==3){
                fprintf(pf2,"%c,",table[i]);
            }
            else{
                fprintf(pf2,"%c",table[i]);
            }
        }
    }
}

int main(){
    char sfile[32]={0};
    char ofile[32]={0};
    char str[100]={0};
    char *word;
    char *part;
    char *section[3];
    char *numa[4];
    char *numb[4];
    int motion,timea,timeb;
    double sec;
    int c,d,e,f=0,i,msec;

    printf("Please enter the subtitle file:");
    if(fgets(sfile,sizeof(sfile),stdin)==NULL){
        printf("Error!\n");
        return 0;
    }
    if(sfile[strlen(sfile)-1]=='\n'){
        sfile[strlen(sfile)-1]=0;
    }
    else{
        while( (f=fgetc(stdin) )!='\n' && f!=EOF ){}
    }
    if( ( pf1=fopen(sfile,"r") )==NULL ){
        printf( "File could not be opened!\n" );
        return 0;
    }

    for(i=0;i<strlen(sfile)-3;i++){
        ofile[i]=sfile[i];
    }
    ofile[strlen(sfile)-3]='o';
    ofile[strlen(sfile)-2]='r';
    ofile[strlen(sfile)-1]='g';
    if( ( pf2=fopen(ofile,"w") )==NULL ){
        printf( "File could not be opened!\n" );
        return 0;
    }

    printf("Forward Shift (1) or Backward Shift (2):");
    scanf("%d",&motion);
    printf("Shift time (sec):");
    scanf("%lf",&sec);
    msec=sec*1000;

    if(motion==1){
        while( fgets(str,sizeof(str),pf1) != NULL){
            if(str[strlen(str)-1]=='\n'){
                str[strlen(str)-1]='\0';
            }
            if(str[0]=='0'){
                c=0;d=0;e=0;
                word=strtok(str," ");
                while(word!=NULL){
                    section[c]=word;
                    word=strtok(NULL," ");
                    c++;
                }
                part=strtok(section[0],":,");
                while(part!=NULL){
                    numa[d]=part;
                    part=strtok(NULL,":,");
                    d++;
                }
                part=strtok(section[2],":,");
                while(part!=NULL){
                    numb[e]=part;
                    part=strtok(NULL,":,");
                    e++;
                }
                timea=atoi(numa[1])*100000+atoi(numa[2])*1000+atoi(numa[3])+msec;
                timeb=atoi(numb[1])*100000+atoi(numb[2])*1000+atoi(numb[3])+msec;
                timePrint(timea);
                fprintf(pf2," --> ");
                timePrint(timeb);
                fprintf(pf2,"\n");
            }
            else{
                fprintf(pf2,"%s\n",str);
            }
        }
    }
    else if(motion==2){
        while( fgets(str,sizeof(str),pf1) != NULL){
            if(str[strlen(str)-1]=='\n'){
                str[strlen(str)-1]='\0';
            }
            if(str[0]=='0'){
                c=0;d=0;e=0;
                word=strtok(str," ");
                while(word!=NULL){
                    section[c]=word;
                    word=strtok(NULL," ");
                    c++;
                }
                part=strtok(section[0],":,");
                while(part!=NULL){
                    numa[d]=part;
                    part=strtok(NULL,":,");
                    d++;
                }
                part=strtok(section[2],":,");
                while(part!=NULL){
                    numb[e]=part;
                    part=strtok(NULL,":,");
                    e++;
                }
                timea=atoi(numa[1])*100000+atoi(numa[2])*1000+atoi(numa[3])-msec;
                timeb=atoi(numb[1])*100000+atoi(numb[2])*1000+atoi(numb[3])-msec;
                timePrint(timea);
                fprintf(pf2," --> ");
                timePrint(timeb);
                fprintf(pf2,"\n");
            }
            else{
                fprintf(pf2,"%s\n",str);
            }
        }
    }
    printf("Done!\n");
}