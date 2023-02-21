#include<stdio.h>
#include<string.h>

#define YELLOW "\x1b[;33;1m"
#define BLUE "\x1b[;34;1m"
#define RESET "\x1b[0;m"

const char *key[]={"break","continue","do","else","for","goto","if","return","switch","while","case","default","char","const","double","enum","extern","float","int8_t","int16_t","int32_t","int64_t","int","long","short","signed","sizeof","static","struct","uint8_t","uint16_t","uint32_t","uint64_t","typedef","union","unsigned","void"};

void compare(char *str){
    int i,s=0,uc=0;
    if(strcmp(str,"for(")==0){
        printf(BLUE"for"YELLOW"( "RESET);
        uc=1;
    }
    if(strcmp(str,"if(")==0){
        printf(BLUE"if"YELLOW"( "RESET);
        uc=1;
    }
    if(strcmp(str,"while(")==0){
        printf(BLUE"while"YELLOW"( "RESET);
        uc=1;
    }
    if(strcmp(str,"sizeof(")==0){
        printf(BLUE"sizeof"YELLOW"( "RESET);
        uc=1;
    }
    if(uc==0){
        for(i=0;i<37;i++){
            if(strcmp(str,(char *)key[i])==0){
                printf(BLUE"%s "RESET,str);
                s=1;
            }
        }
        if(s==0){
            for(i=0;i<strlen(str);i++){
                if(str[i]=='(' || str[i]==')' || str[i]=='{' || str[i]=='}' || str[i]=='[' || str[i]==']'){
                    printf(YELLOW"%c"RESET,str[i]);
                }
                else{
                    printf("%c",str[i]);
                }
            }
            printf(" ");
        }
    }
}

int main(){
    char *word;
    char fileName[32]={0};
    char str[500]={0};
    int i,j,s,c=0,l;
    FILE *pf1=NULL;

    printf("Please enter the file name: ");	
    if(fgets(fileName,sizeof(fileName),stdin)==NULL){
        printf("Error!\n");
        return 0;
    }
    if(fileName[strlen(fileName)-1]=='\n'){
        fileName[strlen(fileName)-1]=0;
    }
    else{
        while( ( c=fgetc(stdin) ) != '\n' && c!=EOF ){}
    }
    if( ( pf1=fopen(fileName,"r") )==NULL ){
        printf( "File could not be opened!\n" );
        return 0;
    }

    while( (fgets(str,sizeof(str),pf1)) != NULL){
        if(str[strlen(str)-1]=='\n'){
            str[strlen(str)-1]='\0';
        }
        for(i=0;i<strlen(str);i++){
            if(str[i]=='\t'){
                printf("\t");
            }
            else if(str[i]==' '){
                printf(" ");
            }
            else{
                break;
            }
        }
        word=strtok(str," \t");
        while(word!=NULL){
            compare(word);
            word=strtok(NULL," \t");
        }
        printf("\n");
    }

    fclose(pf1);
}
