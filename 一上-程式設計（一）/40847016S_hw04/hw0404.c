#include<stdio.h>
#include<stdint.h>
#include<math.h>

int main(){
    int32_t num,i,sumx=0,sumy=0,sumxy=0,sumx2=0,sumy2=0;
    double avrx,avry,b,c;
    printf("Please enter the point number:");
    scanf("%d",&num);
    int32_t x[num],y[num];
    for(i=0;i<num;i++){
        printf("Please enter point %d:",i+1);
        scanf("%d%d",&x[i],&y[i]);
        sumx+=x[i];
        sumy+=y[i];
        sumxy+=x[i]*y[i];
        sumx2+=pow(x[i],2);
        sumy2+=pow(y[i],2);
    }
    avrx=sumx/4;
    avry=sumy/4;
    b=(sumxy-num*avrx*avry)/(sqrt(sumx2-num*pow(avrx,2))*sqrt(sumy2-num*pow(avry,2)))*sqrt(sumy2-num*pow(avry,2))/sqrt(sumx2-num*pow(avrx,2));
    c=avry-b*avrx;
    printf("y = %.2fx + %d\n",b,(int)c);
}