#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include"hw0604test.h"

void multiplication(uint8_t *size,uint8_t *x, uint8_t size_x ,uint8_t *y, uint8_t size_y );

int main(){
        uint8_t s=size_x+size_y;
        uint8_t *size=&s;
        multiplication(size,x, size_x ,y, size_y);
}

void multiplication(uint8_t *size,uint8_t *x, uint8_t size_x ,uint8_t *y, uint8_t size_y ){
        uint8_t a[size_x],b[size_y],total[*size],carry=0;
        for(size_t i=0;i<size_x;i++){  
                a[i]=x[size_x-1-i];
        }  
        for(size_t i=0;i<size_y;i++){  
                b[i]=y[size_y-1-i];
        }
        for(size_t i=0;i<*size;i++){
                total[i]=0;
        }
        for(size_t i=0;i<size_x;i++){  
                for(size_t j=0;j<size_y;j++){  
                        total[j+i]=a[i]*b[j]+carry+total[j+i];  
                }  
        }
        for(size_t i=0;i<*size;i++){  
                total[i]=total[i]+carry;  
                if(total[i]>=10){  
                        carry=total[i]/10;  
                        total[i]=total[i]%10;  
                }  
                else{  
                        carry=0;  
                }
        }
        uint8_t result[*size];
        if(total[*size-1]==0){
                for(size_t i=0;i<*size-1;i++){
                        result[i]=total[*size-i-2];
                        printf("%u",result[i]);
                }
        }
        else{
                for(size_t i=0;i<*size;i++){
                        result[i]=total[*size-i-1];
                        printf("%u",result[i]);
                }
        }
}
