#include<stdio.h>
#include<stdint.h>

int32_t t(int32_t exp){
    int32_t total=1;
    for(int32_t i=1;i<=exp;i++){
        total*=10;
    }
    return total;
}

int main(){
    int32_t num,j,k,exp,sum[3];
    printf("Please enter a number:");
    scanf("%d", &num);
    for(j=0;j<10;j++){
        if(num<t(j)){
            exp=j-1;
            break;
        }
    }
    for(j=0;j<3;j++){
        sum[j]=0;
    }
    int32_t order[exp];
    for(k=0;k<exp;k++){
        order[k]=num/t(exp-k)-num/t(exp-k+1)*10;
        sum[0]+=order[k];
        printf("%d + ",order[k]);
    }
    order[exp]=num/t(0)-num/t(1)*10;
    sum[0]+=order[exp];
    printf("%d = %d\n",order[exp],sum[0]);
    for(k=1;k<3;k++){
        if(sum[k-1]>=10){
            sum[k]=sum[k-1]/10+sum[k-1]-(sum[k-1]/10)*10;
            printf("%d + %d = %d\n",sum[k-1]/10,sum[k-1]-(sum[k-1]/10)*10,sum[k]);
        }
    }
}