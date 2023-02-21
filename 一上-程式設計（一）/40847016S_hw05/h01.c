#include"h01.h"

int32_t add(int32_t a,int32_t b,int32_t c,int32_t d){
        double sum;
        int32_t i,hcf,e,f;
        e=a*d+b*c;
        f=b*d;
        for(i=1;i<=f;i++){  
                if(e%i==0 && f%i==0){
                        hcf=i;
                }  
        }
        e/=hcf;  
        f/=hcf;
        if(f==1){
                printf("%d/%d + %d/%d = %d\n",a,b,c,d,e);
        }
        else{
                printf("%d/%d + %d/%d = %d/%d\n",a,b,c,d,e,f);
        }
}

int32_t sub(int32_t a,int32_t b,int32_t c,int32_t d){
        double total;
        int32_t i,hcf,e,f;
        e=a*d-b*c;
        f=b*d;
        for(i=1;i<=f;i++){  
                if(e%i==0 && f%i==0){
                        hcf=i;
                }  
        }
        e/=hcf;  
        f/=hcf;
        if(f==1){
                printf("%d/%d - %d/%d = %d\n",a,b,c,d,e);
        }
        else{
                printf("%d/%d - %d/%d = %d/%d\n",a,b,c,d,e,f);
        }
}

int32_t mul(int32_t a,int32_t b,int32_t c,int32_t d){
        double result;
        int32_t i,hcf,e,f;
        e=a*c;
        f=b*d;
        for(i=1;i<=f;i++){  
                if(e%i==0 && f%i==0){
                        hcf=i;
                }  
        }
        e/=hcf;  
        f/=hcf;
        if(f==1){
                printf("%d/%d * %d/%d = %d\n",a,b,c,d,e);
        }
        else{
                printf("%d/%d * %d/%d = %d/%d\n",a,b,c,d,e,f);
        }
}

int32_t div(int32_t a,int32_t b,int32_t c,int32_t d){
        double result;
        int32_t i,hcf,e,f;
        e=a*d;
        f=b*c;
        for(i=1;i<=f;i++){  
                if(e%i==0 && f%i==0){
                        hcf=i;
                }  
        }
        e/=hcf;  
        f/=hcf;
        if(f==1){
                printf("%d/%d / %d/%d = %d\n",a,b,c,d,e);
        }
        else{
                printf("%d/%d / %d/%d = %d/%d\n",a,b,c,d,e,f);
        }
}
