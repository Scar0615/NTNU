#include<stdio.h>
#include<stdint.h>

int main(){
        int32_t num[30]={0},dot[20]={0},bamboo[20]={0},character[20]={0},other[20]={0},jl[3]={0};
        int32_t cT=0,cD=0,cB=0,cC=0,cO=0,cj=0,i,j,tmp;
        int32_t pong=0,kong=0,chow=0,pair=0;
        while(1){
                printf("Please enter the tile:");
                scanf("%d",&num[cT]);
                if(num[cT]==-1){
                        break;
                }
                else{
                        if((num[cT]-1)/9==0){
                                dot[cD]=(num[cT]-1)%9+1;
                                cD++;
                        }
                        else if((num[cT]-1)/9==1){
                                bamboo[cB]=(num[cT]-1)%9+1;
                                cB++;
                        }
                        else if((num[cT]-1)/9==2){
                                character[cC]=(num[cT]-1)%9+1;
                                cC++;
                        }
                        else{
                                other[cO]=(num[cT]-1)%9+1;
                                cO++;
                        }
                        cT++;
                }
        }
        //sort
        for(i=0;i<cD;i++){
                for(j=0;j<cD;j++){
                        if(dot[i]<dot[j]){
                                tmp=dot[j];
                                dot[j]=dot[i];
                                dot[i]=tmp;
                        }
                }
        }
        for(i=0;i<cB;i++){
                for(j=0;j<cB;j++){
                        if(bamboo[i]<bamboo[j]){
                                tmp=bamboo[j];
                                bamboo[j]=bamboo[i];
                                bamboo[i]=tmp;
                        }
                }
        }
        for(i=0;i<cC;i++){
                for(j=0;j<cC;j++){
                        if(character[i]<character[j]){
                                tmp=character[j];
                                character[j]=character[i];
                                character[i]=tmp;
                        }
                }
        }
        //judgment preparation
        int32_t jd[cD],jb[cB],jc[cC],jo[cO];
        for(i=0;i<cD;i++){
                jd[i]=dot[i];
        }
        for(i=0;i<cB;i++){
                jb[i]=bamboo[i];
        }
        for(i=0;i<cC;i++){
                jc[i]=character[i];
        }
        for(i=0;i<cO;i++){
                jo[i]=other[i];
        }
        //judgment for kong
        for(i=0;i<cD-3;i++){
                if(jd[i]==jd[i+1] && jd[i+1]==jd[i+2] && jd[i+2]==jd[i+3]){
                        kong++;
                        for(j=i;j<i+4;j++){
                                jd[j]=100+j;
                        }
                }
        }
        for(i=0;i<cB-3;i++){
                if(jb[i]==jb[i+1] && jb[i+1]==jb[i+2] && jb[i+2]==jb[i+3]){
                        kong++;
                        for(j=i;j<i+4;j++){
                                jb[j]=100+j;
                        }
                }
        }
        for(i=0;i<cC-3;i++){
                if(jc[i]==jc[i+1] && jc[i+1]==jc[i+2] && jc[i+2]==jc[i+3]){
                        kong++;
                        for(j=i;j<i+4;j++){
                                jc[j]=100+j;
                        }
                }
        }
        for(i=0;i<cO-3;i++){
                if(jo[i]==jo[i+1] && jo[i+1]==jo[i+2] && jo[i+2]==jo[i+3]){
                        kong++;
                        for(j=i;j<i+4;j++){
                                jo[j]=100+j;
                        }
                }
        }
        //judgment for pong
        for(i=0;i<cD-2;i++){
                if(jd[i]==jd[i+1] && jd[i+1]==jd[i+2]){
                        pong++;
                        for(j=i;j<i+3;j++){
                                jd[j]=200+j;
                        }
                }
        }
        for(i=0;i<cB-2;i++){
                if(jb[i]==jb[i+1] && jb[i+1]==jb[i+2]){
                        pong++;
                        for(j=i;j<i+3;j++){
                                jb[j]=200+j;
                        }
                }
        }
        for(i=0;i<cC-2;i++){
                if(jc[i]==jc[i+1] && jc[i+1]==jc[i+2]){
                        pong++;
                        for(j=i;j<i+3;j++){
                                jc[j]=200+j;
                        }
                }
        }
        for(i=0;i<cO-2;i++){
                if(jo[i]==jo[i+1] && jo[i+1]==jo[i+2]){
                        pong++;
                        for(j=i;j<i+3;j++){
                                jo[j]=200+j;
                        }
                }
        }
        //judgment for pair
        for(i=0;i<cD-2;i++){
                if((jd[i]==jd[i+1] && jd[i+1]!=jd[i+2]) || (jd[i]!=jd[i+1] && jd[i+1]==jd[i+2])){
                        pair++;
                }
        }
        for(i=0;i<cD-3;i++){
                if((jd[i]!=jd[i+1] && jd[i+1]==jd[i+2] && jd[i+2]!=jd[i+3])){
                        pair++;
                }
        }
        for(i=0;i<cB-2;i++){
                if((jb[i]==jb[i+1] && jb[i+1]!=jb[i+2]) || (jb[i]!=jb[i+1] && jb[i+1]==jb[i+2])){
                        pair++;
                }
        }
        for(i=0;i<cB-3;i++){
                if((jb[i]!=jb[i+1] && jb[i+1]==jb[i+2] && jb[i+2]!=jb[i+3])){
                        pair++;
                }
        }
        for(i=0;i<cC-2;i++){
                if((jc[i]==jc[i+1] && jc[i+1]!=jc[i+2]) || (jc[i]!=jc[i+1] && jc[i+1]==jc[i+2])){
                        pair++;
                }
        }
        for(i=0;i<cC-3;i++){
                if((jc[i]!=jc[i+1] && jc[i+1]==jc[i+2] && jc[i+2]!=jc[i+3])){
                        pair++;
                }
        }
        for(i=0;i<cO-2;i++){
                if((jo[i]==jo[i+1] && jo[i+1]!=jo[i+2]) || (jo[i]!=jo[i+1] && jo[i+1]==jo[i+2])){
                        pair++;
                }
        }
        for(i=0;i<cO-3;i++){
                if((jo[i]!=jo[i+1] && jo[i+1]==jo[i+2] && jo[i+2]!=jo[i+3])){
                        pair++;
                }
        }
        //judgment for chow
        for(i=0;i<cD;i++){
                jd[i]=dot[i];
        }
        for(i=0;i<cB;i++){
                jb[i]=bamboo[i];
        }
        for(i=0;i<cC;i++){
                jc[i]=character[i];
        }
        for(i=1;i<9;i++){
                for(j=0;j<cD;j++){
                        if(jd[j]==i){
                                jl[cj]=j;
                                cj++;
                                break;
                        }
                }
                if(cj==3){
                        chow++;
                        for(j=0;j<cj;j++){
                                jd[jl[j]]=100+j;
                        }
                        cj=0;
                }
        }
        for(i=1;i<9;i++){
                for(j=0;j<cB;j++){
                        if(jb[j]==i){
                                jl[cj]=j;
                                cj++;
                                break;
                        }
                }
                if(cj==3){
                        chow++;
                        for(j=0;j<cj;j++){
                                jb[jl[j]]=100+j;
                        }
                        cj=0;
                }
        }
        for(i=1;i<9;i++){
                for(j=0;j<cC;j++){
                        if(jc[j]==i){
                                jl[cj]=j;
                                cj++;
                                break;
                        }
                }
                if(cj==3){
                        chow++;
                        for(j=0;j<cj;j++){
                                jc[jl[j]]=100+j;
                        }
                        cj=0;
                }
        }
        //judgment for win
        if((pong+kong+chow)>=5 && pair>=1){
                printf("Winning Hand\n");
        }
        else{
                printf("Not a Winning Hand\n");
        }
}