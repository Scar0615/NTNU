#include <stdio.h>
#include <stdint.h>

int main(){
    int32_t cn[3],p=0;
    printf("Please enter 3 cards:");
    for(int i=0;i<3;i++){
        scanf("%d", &cn[i]);
        if(cn[i]%13==1){
            if(p<=10){
                p+=11;
            }
            else{
                p+=1;
            }
        }
        else if(cn[i]%13<13 && cn[i]%13>9){
            p+=10;
        }
        else if(cn[i]%13<10 && cn[i]%13>1){
            p+=cn[i]%13;
        }
        else{
            p+=10;
        }
    }
    if(p>21){
        printf("bust\n");
    }
    else{
        printf("%d\n", p);
    }
}
