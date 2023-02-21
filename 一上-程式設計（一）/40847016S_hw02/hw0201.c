#include <stdio.h>
#include <stdint.h>

int main(){
	int32_t a,b,c,max,other;
	printf("Please enter 3 edge lengths (integer) of a triangle:");
	scanf("%d%d%d",&a,&b,&c);
	
	if(a>b && a>c){
		max=a;
		other=b*b+c*c;
	}
	else if(b>a && b>c){
		max=b;
		other=a*a+c*c;
	}
	else{
		max=c;
		other=b*b+a*a;
	}

	if(max>=(a+b)){
		printf("The triangle is not exist.\n");
		return 0;
	}
	else if(max*max>other){
		printf("3\n");
	}
	else if(max*max==other){
		printf("2\n");
	}
	else{
		printf("1\n");
	}
	return 0;
}
