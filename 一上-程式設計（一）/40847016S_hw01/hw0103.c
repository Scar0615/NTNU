#include <stdio.h>
#include <stdint.h>

int main(){
    int32_t x1,y1,x2,y2,x3,y3,a,b;
    printf("$ ./hw0103\nPlease enter the 1st point:");
    scanf("%d%d", &x1, &y1);
    printf("Please enter the 2nd point:");
    scanf("%d%d", &x2, &y2);
    a=(y2-y1)/(x2-x1);
    b=y1-a*x1;
    printf("Please enter x of the 3rd point:");
    scanf("%d", &x3);
    y3=a*x3+b;
    printf("y of the 3rd point: %d\n", y3);
}