#include<math.h>
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<getopt.h>
#include<sys/types.h>
#include<sys/stat.h>

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

struct option long_options[] = {  
    { "write",  0, NULL, 'w' }, 
    { "extract", 0, NULL, 'e' },
    { "bits", 0, NULL, 'b' },
    {  0, 0, 0, 0},  
};

int main(int argc,char *argv[]){
    int c,i,j,ci;
    int ow=0,oe=0,ob=0,index=0;
    while( ( c=getopt_long(argc,argv,"web",long_options,&index) )!=-1 ){
        switch(c){
            case 'w':ow=1;break;
            case 'e':oe=1;break;
            case 'b':ob=1;break;
        }
    }

    FILE *pf1=NULL;
    FILE *pf2=NULL;
    FILE *pf3=NULL; //w:new bmp e:new data

    if( ( pf1=fopen(argv[argc-2],"rb") )==NULL ){ //bmp
        printf("File1 could not be opened!\n");
        return 0;
    }
    if( ( pf2=fopen(argv[argc-1],"rb") )==NULL ){ //secret data
        printf("File2 could not be opened!\n");
        return 0;
    }
    //secret檔案的大小
    struct stat st;
    stat(argv[argc-1], &st);
    int size = st.st_size;
    sBmpheader header;
    fread(&header,sizeof(header),1,pf1);
    header.reserve=size;
    //檢查空間是否足夠做隱藏
    int ubit=1; //the number of bits in option b
    if(ob==1){
        //find --bits=N
        for(i=0;i<argc;i++){
            for(j=0;j<strlen(argv[i]);j++){
                if(argv[i][j]=='='){ci=i;}
            }
        }
        //turn --bits=N into 0N
        for(i=0;i<7;i++){argv[ci][i]='0';}
        //turn 0N into integer and check the validity of N
        ubit=atoi(argv[ci]);
        if(ubit<1 || ubit>8){
            printf("The value of N is invalid.The range of N is 1~8.\n");
            return 0;
        }
    }
    int sc; //SizeCheck
    sc=header.bitmap_size*ubit/8;
    if(size>sc){
        printf("The size of secret data is too big.\n");
        return 0;
    }

    int time=8/ubit; //每個secret data的byte需要幾個bmp的byte
    uint8_t frag[time]; //fragment
    int rs; //記錄共處理多少byte的secret data
    uint8_t pixel;
    uint8_t store;
    if(ow==1){
        if( ( pf3=fopen("save.jpg","wb") )==NULL ){ //bmp
            printf("File3 could not be opened!\n");
            return 0;
        }

        rs=0;
        fwrite(&header,sizeof(header),1,pf3);
        while(rs<size){
            for(i=0;i<time;i++){
                frag[i]=0;
            }
            rs++;
            //對secret data做分解
            fread(&store,1,1,pf2);
            frag[0]=store/pow( 2,ubit*(time-1) );
            for(i=1;i<time;i++){
                frag[i]=store/pow( 2,ubit*(time-i-1) ) - ( store/pow( 2,ubit*(time-i) ) )*pow(2,ubit);
            }
            for(i=0;i<time;i++){
                fread(&pixel,1,1,pf1);
                pixel=( pixel/pow(2,ubit) )*pow(2,ubit)+frag[i];
                fwrite(&pixel,1,1,pf3);
            }
            rs++;
        }
        while(1){
            fread(&pixel,1,1,pf1);
            if(feof(pf1)){break;}
            fwrite(&pixel,1,1,pf3);
        }
        fclose(pf3);
        remove(argv[argc-2]);
        rename("save.jpg", argv[argc-2]);
    }

    if(oe==1){
        if( ( pf3=fopen("save.txt","wb") )==NULL ){ //data
            printf("File3 could not be opened!\n");
            return 0;
        }

        rs=0;
        while(rs<size){
            for(i=0;i<time;i++){
                frag[i]=0;
            }
            //從bmp抓取片段
            for(i=0;i<time;i++){
                fread(&pixel,1,1,pf1);
                frag[i]=pixel - ( pixel/pow(2,ubit) )*pow(2,ubit);
            }
            //將片段轉化並存入secret data
            store=0;
            for(i=0;i<time;i++){
                store+=frag[i]*pow( 2,ubit * (time-i-1) );
            }
            fwrite(&store,1,1,pf3);
            rs++;
        }

        fclose(pf3);
        remove(argv[argc-1]);
        rename("save.txt", argv[argc-1]);
    }

    printf("Done!\n");
    fclose(pf1);
    fclose(pf2);
    return 0;
}