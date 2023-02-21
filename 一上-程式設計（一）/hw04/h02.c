#include"h02.h"

void oddDisk(int32_t num){
        int32_t i,times,state=1;
        int32_t a[num],b[num],c[num];
        int32_t ma=num,mb=0,mc=0; //該柱的碟子數量
        times=(pow(2,num)-1)/3;
        if(times*3!=pow(2,num)-1){
                times++;
        }
        for(i=0;i<num;i++){
                a[i]=num-i;
                b[i]=0;
                c[i]=0;
        }
        for(i=pow(2,num)-1;i>0;i--){
                switch(state){
                        case 1:
                        //A->C
                        if(a[ma-1]<c[mc-1] || c[0]==0){
                                mc++;
                                c[mc-1]=a[ma-1];
                                a[ma-1]=0; 
                                ma--;
                                printf("move disk %d to rod 3\n",c[mc-1]);
                        }
                        //C->A
                        else if(a[ma-1]>c[mc-1] || a[0]==0){
                                ma++;
                                a[ma-1]=c[mc-1];
                                c[mc-1]=0; 
                                printf("move disk %d to rod 1\n",a[ma-1]);
                                mc--;
                        }
                        state=2;
                        break;
                        case 2:
                        //A->B
                        if(a[ma-1]<b[mb-1] || b[0]==0){
                                mb++;
                                b[mb-1]=a[ma-1];
                                a[ma-1]=0;
                                printf("move disk %d to rod 2\n",b[mb-1]);
                                ma--;
                        }
                        //B->A
                        else if(b[mb-1]<a[ma-1] || a[0]==0){
                                ma++;
                                a[ma-1]=b[mb-1];
                                b[mb-1]=0;
                                printf("move disk %d to rod 1\n",a[ma-1]);
                                mb--;
                        }
                        state=3;
                        break;
                        case 3:
                        //B->C
                        if(b[mb-1]<c[mc-1] || c[0]==0){
                                mc++;
                                c[mc-1]=b[mb-1];
                                b[mb-1]=0;
                                printf("move disk %d to rod 3\n",c[mc-1]);
                                mb--;
                        }
                        //C->B
                        else if(c[mc-1]<b[mb-1] || b[0]==0){
                                mb++;
                                b[mb-1]=c[mc-1];
                                c[mc-1]=0;
                                printf("move disk %d to rod 2\n",b[mb-1]);
                                mc--;
                        }
                        state=1;
                        break;
                }
        }
}

void evenDisk(int32_t num){
        int32_t i,times,state=1;
        int32_t a[num],b[num],c[num];
        int32_t ma=num,mb=0,mc=0; //該柱的碟子數量
        times=(pow(2,num)-1)/3;
        if(times*3!=pow(2,num)-1){
                times++;
        }
        for(i=0;i<num;i++){
                a[i]=num-i;
                b[i]=0;
                c[i]=0;
        }
        for(i=pow(2,num)-1;i>0;i--){
                switch(state){
                        case 1:
                        //A->B
                        if(a[ma-1]<b[mb-1] || b[0]==0){
                                mb++;
                                b[mb-1]=a[ma-1];
                                a[ma-1]=0; 
                                ma--;
                                printf("move disk %d to rod 2\n",b[mb-1]);
                        }
                        //B->A
                        else if(a[ma-1]>b[mc-1] || a[0]==0){
                                ma++;
                                a[ma-1]=b[mb-1];
                                b[mb-1]=0; 
                                printf("move disk %d to rod 1\n",a[ma-1]);
                                mb--;
                        }
                        state=2;
                        break;
                        case 2:
                        //A->C
                        if(a[ma-1]<c[mc-1] || c[0]==0){
                                mc++;
                                c[mc-1]=a[ma-1];
                                a[ma-1]=0;
                                printf("move disk %d to rod 2\n",c[mc-1]);
                                ma--;
                        }
                        //C->A
                        else if(c[mc-1]<a[ma-1] || a[0]==0){
                                ma++;
                                a[ma-1]=c[mc-1];
                                c[mc-1]=0;
                                printf("move disk %d to rod 1\n",a[ma-1]);
                                mc--;
                        }
                        state=3;
                        break;
                        case 3:
                        //B->C
                        if(b[mb-1]<c[mc-1] || c[0]==0){
                                mc++;
                                c[mc-1]=b[mb-1];
                                b[mb-1]=0;
                                printf("move disk %d to rod 3\n",c[mc-1]);
                                mb--;
                        }
                        //C->B
                        else if(c[mc-1]<b[mb-1] || b[0]==0){
                                mb++;
                                b[mb-1]=c[mc-1];
                                c[mc-1]=0;
                                printf("move disk %d to rod 2\n",b[mb-1]);
                                mc--;
                        }
                        state=1;
                        break;
                }
        }
}