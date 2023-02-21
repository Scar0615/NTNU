#include<stdio.h>
#include<stdbool.h>
#include<string.h>

bool match(char *pattern,char *string){
    if(*pattern=='\0' && *string=='\0'){
        return true;
    }
    if(*pattern=='?' || *pattern==*string){
        return match(pattern+1,string+1);
    }
    if(*pattern=='*'){
        return match(pattern+1,string) || match(pattern,string+1);
    }
    return false;
}

int main(){
    char str[520],pat[100];
    char *take[20];
    char *ptr;
    int i,c=0,state[200];
    printf("Please enter the pattern:");
    fgets(pat,100,stdin);
    printf("Please enter the string:");
    fgets(str,520,stdin);
    printf("Result:");
    ptr=strtok(str," ");
    while(ptr!=NULL){
        take[c++]=ptr;
        ptr=strtok(NULL," ");
    }
    for(i=0;i<c;i++){
        if(match(pat,take[i])){
            printf("%s ",take[i]);
        }
    }
}