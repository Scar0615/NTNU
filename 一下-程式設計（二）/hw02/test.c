#include"test.h"

int fibonacci(int n){
    if(n<=1)
    return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int factorial(int s){
    int result = 1,i;
    for(i=1;i<=s;i++){
        result*=i;
    }
    return result; 
}