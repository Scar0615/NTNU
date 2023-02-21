#include <stdio.h>
#include <stdint.h>
#include "h03.h"

int main() {
  int mod,num;
  printf("Please enter the modulus:");
  scanf("%d",&mod);
  printf("Please enter the number:");
  scanf("%d",&num);
  if(coprime(mod,num)!=1){
    printf("No Inverse\n");
  }
  else{
    printf("The modular multiplicative inverse of %d is %d.\n",num,mmi(mod,num));
  }
}
