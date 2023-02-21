#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

struct _sHeader{
    char bm[2];
    uint32_t size;
    uint32_t reserve;
    uint32_t offset;
    uint32_t header_size;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bpp;
    uint32_t compression;
    uint32_t bitmap_size;
    uint32_t hres;
    uint32_t vres;
    uint32_t used;
    uint32_t important;
}__attribute__ ((__packed__));

typedef struct _sHeader sHeader;

int main(){
    char filename1[50]={0};
    char filename2[50]={0};

    FILE *pf1=NULL;
    FILE *pf2=NULL;

    int a,b,i;

    printf("Please enter the input image name:");
    scanf("%s",filename1);
    printf("Please enter the output image name:");
    scanf("%s",filename2);
    
    if(fgets(filename1,sizeof(filename1),stdin)==NULL){
        printf("Error Input!\n");
        return 0;
    }
    if(filename1[strlen(filename1)-1]=='\n'){
        filename1[strlen(filename1)-1]=0;
    }
    else{
        while( (a=fgetc(stdin) )!='\n' && a!=EOF ){}
    }
    if( ( pf1=fopen(filename1,"rb") )==NULL ){
        printf("File1 could not be opened!\n");
        return 0;
    }
    
    if(fgets(filename2,sizeof(filename2),stdin)==NULL){
        printf("Error Input!\n");
        return 0;
    }
    if(filename2[strlen(filename2)-1]=='\n'){
        filename2[strlen(filename2)-1]=0;
    }
    else{
        while( (b=fgetc(stdin) )!='\n' && b!=EOF ){}
    }
    if( ( pf2=fopen(filename2,"wb") )==NULL ){
        printf("File2 could not be opened!\n");
        return 0;
    }

    sHeader	header;
    fread(&header,sizeof(header),1,pf1);
    header.bpp=16;
    header.size=header.size/3*2;
    header.bitmap_size-=header.size/3;
    fwrite(&header,sizeof(header),1,pf2);

    while( !feof(pf1) ){
        uint8_t take[header.width*3];
        uint16_t color[header.width];
        fread(take, 1, header.width*3, pf1);
        for(i=0;i<header.width*3;i+=3){
            color[i/3]=take[i]*take[i+1]*take[i+2]/256;
        }
        fwrite(color,1,header.width,pf2);
    }

    fclose( pf1 );
    fclose( pf2 );

    return 0;
}