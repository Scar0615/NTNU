#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>

struct head{
    char bm[2];
    uint32_t size;
    uint32_t reserve;
    uint32_t offset;
    uint32_t header_size;
    uint32_t width;
    uint32_t height;
    uint16_t plane;
    uint16_t bpp;
    uint32_t compression;
    uint32_t bitmap_size;
    uint32_t hres;
    uint32_t vres;
    uint32_t used;
    uint32_t important;
}__attribute__ ((__packed__));

struct pixel{
	uint8_t b;
	uint8_t g;
	uint8_t r;
}__attribute__ ((__packed__));

typedef struct head sHeader;
typedef struct pixel sPixel;

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
    if( ( pf1=fopen(filename,"wb") )==NULL ){
        printf("File1 could not be opened!\n");
        return 0;
    }
    //建立畫布屬性
    sHeader header;
    header.bm[0]='B';header.bm[1]='M';
    header.size=2359350;
    header.reserve=0;
    header.offset=54;
    header.header_size=40;
    header.width=1024;
    header.height=768;
    header.plane=1;
    header.bpp=24;
    header.compression=0;
    header.bitmap_size=2359296;
    header.hres=9449;
    header.vres=9449;
    header.used=0;
    header.important=0;
    fwrite(&header,sizeof(header),1,pf1);
    
    sPixel pixel[768][1024];
    //對輸入進行字串處理
    char rgb1[15]={0};
    char rgb2[15]={0};
    char rgb3[15]={0};
    char rgb4[15]={0};
    char *cut;
    char *element[3];
    int order;
    printf("Please enter (R,G,B) in the top left pixel:");
    scanf("%s",rgb1);
    order=0;
    cut=strtok(rgb1,",");
    while(cut!=NULL){
        element[order]=cut;
        cut=strtok(NULL,",");
        order++;
    }
    pixel[767][0].r=atoi(element[0]);
    pixel[767][0].g=atoi(element[1]);
    pixel[767][0].b=atoi(element[2]);

    printf("Please enter (R,G,B) in the top right pixel:");
    scanf("%s",rgb2);
    order=0;
    cut=strtok(rgb2,",");
    while(cut!=NULL){
        element[order]=cut;
        cut=strtok(NULL,",");
        order++;
    }
    pixel[767][1023].r=atoi(element[0]);
    pixel[767][1023].g=atoi(element[1]);
    pixel[767][1023].b=atoi(element[2]);

    printf("Please enter (R,G,B) in the bottom left pixel:");
    scanf("%s",rgb3);
    order=0;
    cut=strtok(rgb3,",");
    while(cut!=NULL){
        element[order]=cut;
        cut=strtok(NULL,",");
        order++;
    }
    pixel[0][0].r=atoi(element[0]);
    pixel[0][0].g=atoi(element[1]);
    pixel[0][0].b=atoi(element[2]);

    printf("Please enter (R,G,B) in the bottom right pixel:");
    scanf("%s",rgb4);
    order=0;
    cut=strtok(rgb4,",");
    while(cut!=NULL){
        element[order]=cut;
        cut=strtok(NULL,",");
        order++;
    }
    pixel[0][1023].r=atoi(element[0]);
    pixel[0][1023].g=atoi(element[1]);
    pixel[0][1023].b=atoi(element[2]);
    //進行linear interpolation的運算
    int i,j;
    //先做左上到左下＆右上到右下兩條垂直線 pixel(0,0)->pixel(0,767)
    for(i=0;i<768;i++){
        pixel[i][0].b=pixel[0][0].b+(pixel[767][0].b-pixel[0][0].b)*i/767;
        pixel[i][0].g=pixel[0][0].g+(pixel[767][0].g-pixel[0][0].g)*i/767;
        pixel[i][0].r=pixel[0][0].r+(pixel[767][0].r-pixel[0][0].r)*i/767;
        pixel[i][1023].b=pixel[0][1023].b+(pixel[767][1023].b-pixel[0][1023].b)*i/767;
        pixel[i][1023].g=pixel[0][1023].g+(pixel[767][1023].g-pixel[0][1023].g)*i/767;
        pixel[i][1023].r=pixel[0][1023].r+(pixel[767][1023].r-pixel[0][1023].r)*i/767;
    }
    //再來做每一條水平線
    for(i=0;i<768;i++){
        for(j=0;j<1024;j++){
            pixel[i][j].b=pixel[i][0].b+(pixel[i][1023].b-pixel[i][0].b)*j/1023;
            pixel[i][j].g=pixel[i][0].g+(pixel[i][1023].g-pixel[i][0].g)*j/1023;
            pixel[i][j].r=pixel[i][0].r+(pixel[i][1023].r-pixel[i][0].r)*j/1023;
        }
    }

    fwrite( pixel, 1, header.bitmap_size, pf1 );

    fclose(pf1);
    printf("Done!\n");
    return 0;
}