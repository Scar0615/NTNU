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
    
    if( ( pf1=fopen(filename1,"rb") )==NULL ){
        printf("File1 could not be opened!\n");
        return 0;
    }

    printf("Please enter the output image name:");
    scanf("%s",filename2);
    
    if( ( pf2=fopen(filename2,"wb") )==NULL ){
        printf("File2 could not be opened!\n");
        return 0;
    }

    sHeader	header;
    fread(&header,sizeof(header),1,pf1);
    header.bpp=16;
    header.offset=54+12;
    header.bitmap_size=header.bitmap_size/3*2;
    header.size=header.bitmap_size+54+12;
    header.compression=3;
    fwrite(&header,sizeof(header),1,pf2);

    uint32_t mask[3]={0x0000f800,0x000007e0,0x0000001f};
    fwrite(mask,12,1,pf2);

    for(int i=0;i<(header.width*header.height);i++){
        uint8_t original[3];
        uint16_t modified;
        fread(original,1,3,pf1);
        //blue
        original[0]=original[0] >> 3;
        //green
        original[1]=original[1] >> 2;
        //red
        original[2]=original[2] >> 3;
        modified=(original[2]<<11) | (original[1]<<5) | (original[0]);
        fwrite(&modified,2,1,pf2);
    }

    printf("The picture has successfully transformed!\n");

    fclose( pf1 );
    fclose( pf2 );

    return 0;
}