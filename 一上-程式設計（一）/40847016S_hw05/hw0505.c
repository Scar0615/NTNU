#include<stdio.h>
#include<stdint.h>
#include"h01.h"

int main(){
        int32_t a,b,c,d,e,f;
        printf("Please enter 1st rational number:");
        scanf("%d%d",&a,&b);
        printf("Please enter 2nd rational number:");
        scanf("%d%d",&c,&d);
        add(a,b,c,d);
        sub(a,b,c,d);
        mul(a,b,c,d);
        div(a,b,c,d);
}