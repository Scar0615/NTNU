#include<stdio.h>
#include<string.h>
#include<stdint.h>

struct role{
    uint32_t merit;//功勳
    uint8_t leadership;//統帥
    uint8_t operation;//營運
    uint8_t information;//情報
    uint8_t manuever;//機動
    uint8_t attack;//攻擊
    uint8_t defence;//防禦
    uint8_t land;//陸戰
    uint8_t air;//空戰
    uint16_t pwork;//政治工作
    uint16_t iwork;//情報工作
    uint16_t mwork;//軍事工作
}__attribute__ ((__packed__));

typedef struct role sRole;

int main(){
    FILE *pf1=NULL;
    FILE *pf2=NULL;
    char filename[32]={0};
    //輸入檔名
    printf("Please enter the game data file: ");

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
    //修改表面數值
    uint8_t data[12886];
    fread(data,1,12886,pf1);
    fwrite(data,1,12886,pf2);

    sRole role;
    fread(&role,1,sizeof(role),pf1);
    
    uint32_t putt;
    printf("Please enter the value of merit:");
    scanf("%u",&putt);
    role.merit = putt;

    uint8_t put;
    printf("Please enter the value of leadership(0-255):");
    scanf("%hhu",&put);
    role.leadership = put;
    printf("Please enter the value of operation(0-255):");
    scanf("%hhu",&put);
    role.operation = put;
    printf("Please enter the value of information(0-255):");
    scanf("%hhu",&put);
    role.information = put;
    printf("Please enter the value of manuever(0-255):");
    scanf("%hhu",&put);
    role.manuever = put;
    printf("Please enter the value of attack(0-255):");
    scanf("%hhu",&put);
    role.attack = put;
    printf("Please enter the value of defence(0-255):");
    scanf("%hhu",&put);
    role.defence = put;
    printf("Please enter the value of land war(0-255):");
    scanf("%hhu",&put);
    role.land = put;
    printf("Please enter the value of air war(0-255):");
    scanf("%hhu",&put);
    role.air = put;

    uint16_t puts;
    printf("Please enter the value of political work(0-65535):");
    scanf("%hu",&puts);
    role.pwork = puts;
    printf("Please enter the value of informative work(0-65535):");
    scanf("%hu",&puts);
    role.iwork = puts;
    printf("Please enter the value of military work(0-65535):");
    scanf("%hu",&puts);
    role.mwork = puts;
    
    fwrite(&role,1,sizeof(role),pf2);
    //修改真正的遊戲數值
    uint8_t info[60477];
    fread(info,1,60477,pf1);
    fwrite(info,1,60477,pf2);
    fwrite(&role,1,sizeof(role),pf2);

    uint8_t remain;
    while(!feof(pf1)){
        fread(&remain,1,1,pf1);
        fwrite(&remain,1,1,pf2);
    }
    //結束
    printf("Done!\n");
    fclose(pf1);
    fclose(pf2);

    remove(filename);
    rename("save.txt", filename);
    return 0;
}