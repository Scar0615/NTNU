#include <stdio.h>
#include <stdint.h>

int main(){
    int32_t enter=0,sum=0;
    printf("$ ./hw0102\n");
    printf("Please enter a 5-digits integer: \n");
    scanf("%d", &enter);
    sum=enter/10000+enter/1000-(enter/10000)*10+enter/100-(enter/1000)*10+enter/10-(enter/100)*10+enter-(enter/10)*10;
    printf("Result: %d\n", sum);
}