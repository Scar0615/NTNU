#include"h03.h"

int32_t coprime(int32_t x, int32_t y) {
  int32_t tmp;
  while (x % y != 0) {
  tmp = y;
  y = x % y;
  x = tmp;
  }
  return y;
}

int32_t mmi(int32_t mod,int32_t num){
  int32_t i=1;
  while((num*i)%mod!=1){
    i++;
  }
  return i;
}