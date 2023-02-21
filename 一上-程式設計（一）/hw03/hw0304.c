#include<stdio.h>
#include<stdint.h>

int32_t t(int32_t ex){
    int32_t total=1;
    for(int32_t i=1;i<=ex;i++){
        total*=2;
    }
    return total;
}

int32_t f(int32_t exp){
    int32_t total=1;
    for(int32_t i=1;i<=exp;i++){
        total*=10;
    }
    return total;
}

int main(){
    int32_t num,perNum[13],exp,i,j,digit,sum[3];
    printf("Please enter an integer (1-50000000):");
    scanf("%d", &num);
    for(i=0;i<13;i++){
        perNum[i]=t(i+1)*(t(i+2)-1);
        if(perNum[i]>num){
            exp=i-1;
            break;
        }
    }
    if(num>=6){
        printf("6");
    }
    for(int32_t k=0;k<=exp;k++){
        for(j=0;j<10;j++){
            if(perNum[k]<f(j)){
                digit=j;
                break;
            }
        }
        for(int32_t l=0;l<digit;l++){
            sum[0]+=perNum[k]/f(l)-(perNum[k]/f(l+1))*10;
        }
        for(int32_t m=1;m<3;m++){
            if(sum[m-1]>=10){
                sum[m]=sum[m-1]/10+sum[m-1]-(sum[m-1]/10)*10;
                if(sum[m]==1){
                    if(k==7 || k==9){
                        printf(" ");
                    }
                    else{
                        printf("\n%d",perNum[k]);
                    }
                }
            }
        }
        for(int32_t n=0;n<3;n++){
            sum[n]=0;
        }
    }
    printf("\n");
}