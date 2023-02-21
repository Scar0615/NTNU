#include<stdio.h>
#include<stdint.h>

int main(){
    int32_t mia;
    double aar;
    double fmax,favr,fmin;
    double yrMax[20],yrAvr[20],yrMin[20];
    double mrMax[240],mrAvr[240],mrMin[240];
    double max[20],avr[20],min[20];
    printf("Please enter your monthly investment amount (1000-50000):");
    scanf("%d", &mia);
    if(mia<1000 || mia>50000){
        printf("Error Input!\n");
        return 0;
    }
    printf("Please enter your anticipated annual rate:");
    scanf("%lf", &aar);
    for(int32_t h=0;h<240;h++){
        mrMax[h]=1+(aar/1200)*1.03;
        mrAvr[h]=1+(aar/1200);
        mrMin[h]=1+(aar/1200)*0.97;
    }
    for(int32_t i=1;i<240;i++){
        for(int32_t j=1;j<=i;j++){
            mrMax[i]*=1+(aar/1200)*1.03;
            mrAvr[i]*=1+(aar/1200);
            mrMin[i]*=1+(aar/1200)*0.97;
        }
    }
    for(int32_t q=1;q<=20;q++){
        for(int32_t r=0;r<12*q;r++){
            yrMax[q-1]=yrMax[q-1]+mrMax[r];
            yrAvr[q-1]=yrAvr[q-1]+mrAvr[r];
            yrMin[q-1]=yrMin[q-1]+mrMin[r];
        }
    }
    for(int32_t n=0;n<20;n++){
        max[n]=mia*yrMax[n];
        avr[n]=mia*yrAvr[n];
        min[n]=mia*yrMin[n];
        printf("Year %d: %d (max) %d (average) %d (min)\n",n+1,(int)max[n],(int)avr[n],(int)min[n]);
    }
}