#include <stdio.h>
#include <stdint.h>

double f(int32_t num){
    int32_t x=1;
    for(int32_t i=1;i<=num;i++){
        x=x*i;
    }
    return x;
}

int main(){
    int32_t k,j;
    float e=1;
    printf("k-th order Taylor polynomial for e\nPlease enter k:");
    scanf("%d", &k);
    for(j=1;j<=k;j++){
        e += 1/f(j);
    }
    printf("%f\n",e);
}