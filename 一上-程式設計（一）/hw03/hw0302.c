#include <stdio.h>
#include <stdint.h>

int32_t f(int32_t num){
    int32_t x=1;
    for(int i=1;i<=num;i++){
        x=x*i;
    }
    return x;
}

int main(){
    int32_t num;
	printf("Please enter the height of a Pascal's Triangle (1-10):");
	scanf("%d", &num);
    if(num<1 || num>10){
        printf("Error Input!\n");
        return 0;
    }
	int32_t a[num];

	for(int32_t i=0;i<num;i++){
        for(int32_t j=0;j<i+1;j++){
            a[j]=f(i)/( f(j)*f(i-j) );
            printf("%d ", a[j]);
        }
        printf("\n");
    }
}