#include"mystring.h"

char *mystrchr(const char *s, int c){
    while(*s!='\0'){
	  	if(*s==c){
			return (char *)s;
		}
        s++;	   
	}
    return NULL;
}

char *mystrrchr(const char *s, int c){
    char *temp=NULL;
    if(s==NULL){
        return NULL;
    }
    temp=(char *)s;
    while(*temp){
        temp++;
    }
    while(temp!=s && *temp!=c){
        temp--;
    }
    if(temp!=s){
        return temp;
    }
    else{
        return NULL;
    }
}

size_t mystrspn(const char *s, const char *accept){
    int i,j,count=0,state;
    for(i=0;i<strlen(s);i++){
        state=0;
        for(j=0;j<strlen(accept);j++){
            if(s[i]==accept[j]){
                state=1;
            }
        }
        if(state==1){
            count++;
        }
        else{
            return count;
        }
    }
    return 0;
}

size_t mystrcspn(const char *s, const char *reject){
    int count=0,i,j,state;
    for(i=0;i<strlen(s);i++){
        state=0;
        for(j=0;j<strlen(reject);j++){
            if(s[i]==reject[j]){
                state=1;
                return count;
            }
        }
        if(state==0){
            count++;
        }
    }
    return count;
}

char *mystrpbrk(const char *s,const char *accept){
    const char *sc1,*sc2;
    for (sc1=s;*sc1!='\0';sc1++){
        for (sc2=accept;*sc2!='\0';sc2++){
            if (*sc1==*sc2){
                return (char *)sc1;
            }
        }
    }
    return NULL;
}

char *mystrstr(const char *haystack, const char *needle){
    const char *pstr = haystack;
    const char *psub = needle;
    const char *pcur = NULL;
    if (haystack==NULL || needle==NULL){
        return NULL;
    }
    while (*haystack!='\0'){
        pcur = pstr;
        psub = needle;
        while (*needle!='\0'){
            if (*pcur!=*psub){
                break;
            }
            pcur++;
            psub++;
        }
        if (*psub=='\0'){
            return (char *)pstr;
        }
        pstr++;
    }
    return NULL;
}

char *mystrtok(char *s, const char *delim) {
    char *token;
    char *save_ptr;
    if(s==NULL){
        s=save_ptr;
    }
    s+=strspn(s,delim);
    if(*s=='\0'){
        return NULL;
    }
    token=s;
    s=strpbrk(token, delim);
    if(s==NULL){
        save_ptr=strchr(token, '\0');
    }
    else{
        *s='\0';
        save_ptr=s+1;
    }

    return token;
}