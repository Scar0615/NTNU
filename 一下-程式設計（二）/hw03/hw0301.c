#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct team{
    char *name;
    int point;
    int win;
    int draw;
    int lose;
}__attribute__ ((__packed__));

typedef struct team sTeam;

int main(){
    FILE *pf1=NULL;
    char filename[64]={0};
    printf("Please enter the season data file:");
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
    if( ( pf1=fopen(filename,"r") )==NULL ){
        printf("File1 could not be opened!\n");
        return 0;
    }
    //建立20個隊伍的信息
    sTeam team[20];
    team[0].name="Arsenal";
    team[1].name="Leicester";
    team[2].name="Brighton";
    team[3].name="Man City";
    team[4].name="Chelsea";
    team[5].name="Burnley";
    team[6].name="Crystal Palace";
    team[7].name="Huddersfield";
    team[8].name="Everton";
    team[9].name="Stoke";
    team[10].name="Southampton";
    team[11].name="Swansea";
    team[12].name="Watford";
    team[13].name="Liverpool";
    team[14].name="West Brom";
    team[15].name="Bournemouth";
    team[16].name="Man United";
    team[17].name="West Ham";
    team[18].name="Newcastle";
    team[19].name="Tottenham";

    int i;
    for(i=0;i<20;i++){
        team[i].point=0;
        team[i].win=0;
        team[i].draw=0;
        team[i].lose=0;
    }
    //讀取csv檔並處理
    char str[120]={0};
    char *read,*data[5];
    int line=0,count;
    while( (fgets(str,sizeof(str),pf1)) != NULL){
        //讀資料
        if(str[strlen(str)-1]=='\n'){
            str[strlen(str)-1]='\0';
        }
        read=strtok(str,",");
        count=0;
        while(read!=NULL){
            data[count]=read;
            count++;
            if(count>=5){ break; }
            read=strtok(NULL,",");
        } 
        //判斷輸贏
        int thl,tal; //thl = team home location a=away
        for(i=0;i<20;i++){
            if( strcmp(data[1],team[i].name)==0 ){ thl=i; }
            if( strcmp(data[2],team[i].name)==0 ){ tal=i; }
        }
        int a=atoi(data[3]);
        int b=atoi(data[4]);
        if(a>b){
            team[thl].win+=1;
            team[tal].lose+=1;
        }
        else if(a<b){
            team[tal].win+=1;
            team[thl].lose+=1;
        }
        if( strcmp(data[3],data[4])==0 ){
            team[thl].draw+=1;
            team[tal].draw+=1;
        }
    }
    //計算20隊的分數
    for(i=0;i<20;i++){
        team[i].point = team[i].win*3 + team[i].draw;
    }
    //將20隊的積分做排序
    int j,tmp;
    int score[20]={0};
    for(i=0;i<20;i++){
        score[i]=team[i].point;
    }

    for(i=0;i<20;i++){
        for(j=0;j<20;j++){
            if(score[i]>score[j]){
                tmp=score[j];
                score[j]=score[i];
                score[i]=tmp;
            }
        }
    }
    //使用者輸入隊名
    char teamname[32]={0};
    printf("Please enter Team Name:");
    fgets(teamname,sizeof(teamname),stdin);
    if(teamname[strlen(teamname)-1]=='\n'){
        teamname[strlen(teamname)-1]=0;
    }
    else{
        int a;
        while( (a=fgetc(stdin) )!='\n' && a!=EOF ){}
    }
    //檢查該隊伍是否存在
    int location=20;
    for(i=0;i<20;i++){
        if( strcmp(teamname,team[i].name)==0 ){
            location=i;
            break;
        }
    }
    if(location==20){
        printf("No such a team!\n");
        return 0;
    }
    //找到該隊伍的排名
    int rank;
    for(i=0;i<20;i++){
        if(team[location].point==score[i]){
            rank=i+1;
            break;
        }
    }
    //印出該隊伍的成績
    printf("Points: %d\n",team[location].point);
    printf("Win: %d\n",team[location].win);
    printf("Draw: %d\n",team[location].draw);
    printf("Lose: %d\n",team[location].lose);
    printf("Position: %d",rank);
    switch(rank){
        case 1:
        printf("st\n");
        break;
        case 2:
        printf("nd\n");
        break;
        case 3:
        printf("rd\n");
        break;
        default:
        printf("th\n");
        break;
    }

    fclose(pf1);
    return 0;
}