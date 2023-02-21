#include<stdio.h>
#include<stdint.h>

int main(){
        int32_t state,m,n,p,q,i,j,k,x,y;
        printf("Please enter the operation choice (1: add; 2: sub; 3: mul):");
        scanf("%d",&state);
        //1st matrix
        printf("Please enter the size of the 1st matrix:");
        scanf("%d%d",&m,&n);
        int32_t a[m][n];
        
        for(i=0;i<m;i++){
                printf("Please enter the");
                if(i==0){
                        printf(" %dst ",i+1);
                }
                else if(i==1){
                        printf(" %dnd ",i+1);
                }
                else if(i==2){
                        printf(" %drd ",i+1);
                }
                else{
                        printf(" %dth ",i+1);
                }
                printf("row of the 1st matrix:");
                for(j=0;j<n;j++){
                        scanf("%d",&a[i][j]);
                }
        }
        //2nd matrix
        printf("Please enter the size of the 2nd matrix:");
        scanf("%d%d",&p,&q);
        int32_t b[p][q];
        for(i=0;i<p;i++){
                printf("Please enter the");
                if(i==0){
                        printf(" %dst ",i+1);
                }
                else if(i==1){
                        printf(" %dnd ",i+1);
                }
                else if(i==2){
                        printf(" %drd ",i+1);
                }
                else{
                        printf(" %dth ",i+1);
                }
                printf("row of the 1st matrix:");
                for(j=0;j<q;j++){
                        scanf("%d",&b[i][j]);
                }
        }
        //add&sub
        if(state==1 || state==2){
                x=m;
                y=n;
                int32_t c[x][y];
                if(m!=p || n!=q){
                        printf("Error.\nThe size of 2 matrixs are different.\n");
                        return 0;
                }
                if(state==1){
                        for(i=0;i<x;i++){
                                for(j=0;j<y;j++){
                                        c[i][j]=a[i][j]+b[i][j];
                                }
                        }
                }
                else if(state==2){
                        for(i=0;i<x;i++){
                                for(j=0;j<y;j++){
                                        c[i][j]=a[i][j]-b[i][j];
                                }
                        }
                }
                for(i=0;i<x;i++){
                        for(j=0;j<y;j++){
                                printf("%d ",c[i][j]);
                        }
                        printf("\n");
                }
        }
        //mul
        else if(state==3){
                x=m;
                y=q;
                int32_t c[x][y];
                if(n!=p){
                        printf("Error Input.\n");
                        return 0;
                }
                for(i=0;i<x;i++){
                        for(j=0;j<y;j++){
                                c[i][j]=0;
                        }
                }
                for(i=0;i<x;i++){
                        for(j=0;j<y;j++){
                                for(k=0;k<n;k++){
                                        c[i][j]+=a[i][k]*b[k][j];
                                }
                        }
                }
                for(i=0;i<x;i++){
                        for(j=0;j<y;j++){
                                printf("%d ",c[i][j]);
                        }
                        printf("\n");
                }
        }
}