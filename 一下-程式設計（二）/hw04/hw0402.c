#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include<getopt.h>

struct _sJFIF{
    uint16_t soi; //start of image 0xFFD8
    uint16_t marker; //app0段標記 0xFFE0
    uint16_t length; //app0段長度 = 3*(縮圖水平像素)*(縮圖垂直像素)+16
    uint32_t jfif; //jfif的ascii碼
    uint8_t halt;
    uint16_t version; //jpeg版本
    uint8_t unit; //密度單位
    uint16_t xDensity; //水平像素密度
    uint16_t yDensity; //垂直像素密度
    uint8_t xThumb; //縮圖的水平像素
    uint8_t yThumb; //縮圖的垂直像素
}__attribute__ ((__packed__));

typedef struct _sJFIF sJFIF;

struct _sBmpheader{
    char		bm[2];
    uint32_t	size;
    uint32_t	reserve;
    uint32_t	offset;
    uint32_t	header_size;
    uint32_t	width;
    uint32_t	height;
    uint16_t	plane;
    uint16_t	bpp;
    uint32_t	compression;
    uint32_t	bitmap_size;
    uint32_t	hres;
    uint32_t	vres;
    uint32_t	used;
    uint32_t	important;
}__attribute__ ((__packed__));

typedef struct _sBmpheader sBmpheader;

struct _sPixel{
    uint8_t r;
    uint8_t g;
    uint8_t b;
}__attribute__ ((__packed__));

typedef struct _sPixel sPixel;

struct option long_options[] = {  
    { "write",  0, NULL, 'w' }, 
    { "extract", 0, NULL, 'e' }, 
    {  0, 0, 0, 0},  
}; 

int main(int argc,char *argv[]){
    int c,i,j,k; //i,j,k做迴圈用
    int ow=0;
    int oe=0;
    int index=0;
    while( ( c=getopt_long(argc,argv,"we",long_options,&index) )!=-1 ){
        switch(c){
            case 'w':
            ow=1;
            break;
            case 'e':
            oe=1;
            break;
        }
    }
    //處理檔案
    FILE *pf1=NULL; //old jpg
    FILE *pf2=NULL; //bmp
    FILE *pf3=NULL; //new jpg

    sJFIF jpgheader;
    sBmpheader bmpheader;

    uint8_t original[3];
    uint8_t modified[3];
    //w為輸入bmp圖片來取代縮圖
    
    if(ow==1){
        if( ( pf1=fopen(argv[argc-2],"rb") )==NULL ){ //old jpg
            printf("File1 could not be opened!\n");
            return 0;
        }
        if( ( pf2=fopen(argv[argc-1],"rb") )==NULL ){ //bmp
            printf("File2 could not be opened!\n");
            return 0;
        }
        if( ( pf3=fopen("save.jpg","wb") )==NULL ){ //new jpg
            printf("File2 could not be opened!\n");
            return 0;
        }
        fread(&jpgheader,sizeof(jpgheader),1,pf1);
        fread(&bmpheader,sizeof(bmpheader),1,pf2);
        //檢查bmp的size是否合適
        if( bmpheader.width>255 || bmpheader.height>255 || (bmpheader.width*bmpheader.height*3)>65519 ){
            printf("The bmp size is too big for jpg thumbnail.\n");
            return 0;
        }
        //拿掉原縮圖的資訊
        uint8_t jdata[jpgheader.xThumb * jpgheader.yThumb * 3];
        fread(&jdata,jpgheader.xThumb * jpgheader.yThumb * 3,1,pf1);
        //修改jpg的header
        jpgheader.xThumb = bmpheader.width;
        jpgheader.yThumb = bmpheader.height;
        jpgheader.length = bmpheader.width * bmpheader.height * 3 + 16;
        fwrite(&jpgheader,sizeof(jpgheader),1,pf3);
        //傳輸縮圖的資訊
        for(i=0;i<bmpheader.width;i++){
            for(j=0;j<bmpheader.height;j++){
                fread(&original,3,1,pf2);
                for(k=0;k<3;k++){
                    modified[k]=original[2-k];
                }
                fwrite(&modified,3,1,pf3);
            }
        }
        //搬移jpg圖檔的資訊
        uint8_t	cdata[3] = {0};
        while(1){
            fread(cdata,3,1,pf1);
            if(feof(pf1)){break;}
            fwrite(cdata,3,1,pf3);
        }
    }
    //e為抓出縮圖變成bmp檔
    if(oe==1){
        if( ( pf1=fopen(argv[argc-2],"rb") )==NULL ){
            printf("File1 could not be opened!\n");
            return 0;
        }
        if( ( pf2=fopen(argv[argc-1],"wb") )==NULL ){
            printf("File2 could not be opened!\n");
            return 0;
        }
        //處理header的部份
        fread(&jpgheader,sizeof(jpgheader),1,pf1);
        bmpheader.bm[0]='B';bmpheader.bm[1]='M';
        bmpheader.size = ( jpgheader.xThumb * jpgheader.yThumb * 3 )+54;
        bmpheader.reserve=0;
        bmpheader.offset=54;
        bmpheader.header_size=40;
        bmpheader.width = jpgheader.xThumb ;
        bmpheader.height = jpgheader.yThumb ;
        bmpheader.plane=1;
        bmpheader.bpp=24;
        bmpheader.compression=0;
        bmpheader.bitmap_size = jpgheader.xThumb * jpgheader.yThumb * 3 ;
        bmpheader.hres=9449;
        bmpheader.vres=9449;
        bmpheader.used=0;
        bmpheader.important=0;
        fwrite(&bmpheader,sizeof(bmpheader),1,pf2);
        //傳輸圖片的資訊
        for(i=0;i<jpgheader.xThumb;i++){
            for(j=0;j<jpgheader.yThumb;j++){
                fread(&original,3,1,pf1);
                for(k=0;k<3;k++){
                    modified[k]=original[2-k];
                }
                fwrite(&modified,3,1,pf2);
            }
        }
    }

    printf("Done!\n");
    fclose(pf1);
    fclose(pf2);
    fclose(pf3);
    remove(argv[argc-2]);
    rename("save.jpg", argv[argc-2]);
    return 0;
}