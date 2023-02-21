#include<stdio.h>
#include<stdint.h>
#include<math.h>

int main(){
    int32_t deg,x,i,j;
    printf("Please enter the degree:");
    scanf("%d",&deg);
    int32_t co[deg+1],fx[deg+1];
    printf("Please enter the coefficients:");
    for(i=0;i<=deg;i++){
        scanf("%d",&co[i]);
        fx[i]=0;
    }
    printf("Plesae enter x:");
    scanf("%d",&x);
    for(i=0;i<=deg;i++){
        for(j=0;j<=deg;j++){
            fx[i]+=co[j]*pow(x,deg-i-j);
            co[j]=co[j]*(deg-i-j);
        }
    }
    printf("f(%d) = %d\n",x,fx[0]);
    for(i=1;i<=deg;i++){
        printf("f^-%d(%d) = %d\n",i,x,fx[i]);
    }
}