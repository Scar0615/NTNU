#include"h01.h"

//第一個為起始位置 第二個為中間柱 第三個為結束位置
void hanoi(int n,char a,char b,char c){
    if(n==1){
        printf("move disk 1 to rod %c\n",c);
    }
    else{
        hanoi(n-1,a,c,b);
        printf("move disk %d to rod %c\n",n,c);
        hanoi(n-1,b,a,c);
    }
}