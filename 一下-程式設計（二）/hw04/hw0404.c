#include<stdio.h>

#define FOR_EACH(i,s) for(int i=0;i<s;i++)
#define FOR_EACH_STEP(i,s,x) for(int i=0;i<s;i+=x)
#define SUM_OF_ARRAY(s,a,i) s+=a[i]

int main(){
    int size,sum=0;
    printf("Please enter the array size:");
    scanf("%d",&size);
    int a[size];
    FOR_EACH(i,size){
        a[i]=(size-1)*(i+1)/size + 1;
    }
    FOR_EACH(i,size){
        FOR_EACH(j,size){
            if(a[i]<a[j]){
                int tmp;
                tmp=a[i];
                a[i]=a[j];
                a[j]=tmp;
            }
        }
    }
    FOR_EACH_STEP(i,size,2){
        SUM_OF_ARRAY(sum,a,i);
    }
    printf("The sum is %d\n",sum);
}