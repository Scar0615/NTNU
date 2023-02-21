#include <stdio.h>

int main(){
	float num;
	int state;
	printf("Does the scanf function have the return value? Yes\n");
	printf("What is the return type? Integer");
	printf("What is the meaning of the return value?\nThe return value 1 means the user input an number.\nThe return value 0 means the user's input isn't an number.");
	printf("Please enter an integer or a vocabulary:");
	state = scanf("%d", &num);
	printf("Return Value:%d\n",state);
	return 0;
}
