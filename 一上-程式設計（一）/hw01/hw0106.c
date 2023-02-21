#include <stdio.h>
#include <stdint.h>

int main(){
    printf("Does the printf function have the return value?\nYes\n");
    printf("What is the return type?\nInteger\n");
    int32_t count = printf("This is the bonus part.\n");
    printf("%d\n", count);
}