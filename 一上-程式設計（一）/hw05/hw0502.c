#include<stdio.h>
#include<stdint.h>

int32_t gcd(int32_t x, int32_t y);

int main(){
        int32_t n,h,i,j,x,y,z;
        printf("Please enter the variable number:");
        scanf("%d",&n);
        int32_t a[n][n+1],ans[n],cal[n-1];
        for(i=0;i<n;i++){
                printf("Please enter the");
                if(i==0){
                        printf(" %dst equation:",i+1);
                }
                else if(i==1){
                        printf(" %dnd equation:",i+1);
                }
                else if(i==2){
                        printf(" %drd equation:",i+1);
                }
                else{
                        printf(" %dth equation:",i+1);
                }
                for(j=0;j<n+1;j++){
                        scanf("%d",&a[i][j]);
                }
        }
        for(h=0;h<n-1;h++){
                for(i=h+1;i<n;i++){
                        if(a[h][h]<0){
                                x=a[h][h]*(-1);
                        }
                        else{
                                x=a[h][h];
                        }
                        if(a[i][h]<0){
                                y=a[i][h]*(-1);
                        }
                        else{
                                y=a[i][h];
                        }
                        z=gcd(x,y);
                        for(j=0;j<n+1;j++){
                                a[h][j]*=y/z;
                                a[i][j]*=x/z;
                                if((a[h][j]>0 && a[i][j]<0) || (a[h][j]<0 && a[i][j]>0)){
                                        a[i][j]+=a[h][j];
                                }
                                else{
                                        a[i][j]-=a[h][j];
                                }
                                a[h][j]/=y/z;
                        }
                }
        }
        ans[n-1]=a[n-1][n]/a[n-1][n-1];
        for(i=n-2;i>=0;i--){
                for(j=i+1;j<n;j++){
                        cal[n-1-j]=a[i][j]*ans[j];
                        a[i][n]-=cal[n-1-j];
                }
                ans[i]=a[i][n]/a[i][i];
        }
        for(i=0;i<n;i++){
                printf("x%d = %d\n",i+1,ans[i]);
        }
}

int32_t gcd(int32_t x, int32_t y) {
        int32_t tmp;
        while (x % y != 0) {
        tmp = y;
        y = x % y;
        x = tmp;
        }
        return y;
}