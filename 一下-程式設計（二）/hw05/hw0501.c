#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<getopt.h>

//base64 table
char bt[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

//ascii table
int at[]={
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,62,0,0,0,63,52,53,54,55,56,57,58,59,60,61,0,0,
0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,
15,16,17,18,19,20,21,22,23,24,25,0,0,0,0,0,0,26,27,28,
29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,
49,50,51,0,0,0,0
};

char *encode(char *code){
    int len=strlen(code);
    int enclen;
    int i,j;
    //計算char轉成base64後的長度
    if(len%3==0){
        enclen=len*4/3;
    }
    else{
        enclen=(len+1)*4/3;
    }
    //從char轉成base64
    char *enc=malloc(sizeof(char)*enclen+1);
    enc[enclen]='\0';
    for(i=0,j=0;i<len;i+=3,j+=4){
        enc[j]=bt[code[i]>>2];
        enc[j+1]=bt[(code[i]&0x3)<<4 | (code[i+1]>>4)];
        enc[j+2]=bt[(code[i+1]&0xf)<<2 | (code[i+2]>>6)];
        enc[j+3]=bt[code[i+2]&0x3f];
    }
    //加上=
    switch(len%3){
        case 1:
        enc[j-2]='=';
        enc[j-1]='=';
        break;
        case 2:
        enc[j-1]='=';
        break;
    }

    return enc;
}

char *decode(char *code){
    int len=strlen(code);
    int declen;
    int i,j;
    //計算base64轉成char後的長度
    if( strstr(code,"==") ){
        declen=len/4*3-2;
    }
    else if( strstr(code,"=") ){
        declen=len/4*3-1;
    }
    else{
        declen=len/4*3;
    }
    //從base64轉成char
    char *dec=malloc(sizeof(char)*declen+1);
    dec[declen]='\0';
    for(i=0,j=0;i<len;i+=3,j+=4){
        dec[i]=( (char)at[code[j]]<<2 | (char)at[code[j+1]]>>4 );
        dec[i+1]=( (char)at[code[j+1]]<<4 | (char)at[code[j+2]]>>2 );
        dec[i+2]=( (char)at[code[j+2]]<<6 | (char)at[code[j+3]] );
    }
    
    return dec;
}

struct option long_options[] = {  
    { "enc",  0, NULL, 'e' }, 
    { "dec", 0, NULL, 'd' },
    { "output", 0, NULL, 'o' }, 
    {  0, 0, 0, 0},  
};

int main(int argc,char *argv[]){
    int c;
    int index=0;

    FILE *pf1=NULL;
    FILE *pf2=NULL;

    while( ( c=getopt_long(argc,argv,"e:d:o:",long_options,&index) )!=-1 ){
        switch(c){
            case 'e':{
            char obuf[600]; //original buffer
            pf1=fopen(argv[argc-3],"rb");
            pf2=fopen("change","wb");
            while(!feof(pf1)){
                fgets(obuf,sizeof(obuf),pf1);
                obuf[strlen(obuf)-1]='\0';
                char *bbuf=encode(obuf); //base64 buffer
                fprintf(pf2,"%s\n",bbuf);
                for(int i=0;i<strlen(obuf);i++){
                    obuf[i]=0;
                }
            }
            fclose(pf1);
            fclose(pf2);
            }
            break;
            case 'd':{
            char bstr[1000]; //base64 string
            pf1=fopen(argv[argc-3],"rb");
            pf2=fopen("change","wb");
            while(!feof(pf1)){
                fgets(bstr,sizeof(bstr),pf1);
                bstr[strlen(bstr)-1]='\0';
                char *ostr=decode(bstr); //original string
                fprintf(pf2,"%s\n",ostr);
                for(int i=0;i<strlen(bstr);i++){
                    bstr[i]=0;
                }
            }
            fclose(pf1);
            fclose(pf2);
            }
            break;
            case 'o':
            rename("change",argv[argc-1]);
            printf("Done!\n");
            return 0;
            break;
        }
    }
}