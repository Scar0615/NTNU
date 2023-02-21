#include<stdio.h>
#include<stdint.h>
#include"h01.h"

int main(){
	int32_t num;
	printf("Please enter the disk number(2-20):");
	scanf("%d",&num);
	hanoi(num,'1','2','3');
	return 0;
}