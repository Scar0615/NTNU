#include<stdio.h>

typedef struct node{
    int leftCity;
    int rightCity;
    int price;
    int govNum;//州政府編號
}info;

int main(){
    int v,e,c,d,n;
    int i,j,k;

    //輸入測資
    scanf("%d%d%d",&v,&e,&c);
    int cityGovNum[v];
    for(i=0;i<v;i++){
        scanf("%d",&cityGovNum[i]);
    }
    info road[10];
    for(i=0;i<e;i++){
        scanf("%d%d%d%d",&road[i].leftCity,&road[i].rightCity,&road[i].price,&road[i].govNum);
    }
    scanf("%d",&d);
    scanf("%d",&n);
    int visitOrder[n];
    for(i=0;i<n;i++){
        scanf("%d",&visitOrder[i]);
    }

    //建立二維陣列來儲存城市之間的過路費
    int costMap[v][v];
    //先假設所有城市都不相連
    for(i=0;i<v;i++){
        for(j=0;j<v;j++){
            costMap[i][j]=999;
        }
    }
    //自己不會連到自己
    for(i=0;i<v;i++){
        costMap[i][i]=0;
    }
    //找出有關聯的
    for(i=0;i<e;i++){
        if( cityGovNum[ road[i].leftCity-1 ] != road[i].govNum ){
            costMap[ road[i].leftCity-1 ][ road[i].rightCity-1 ] = road[i].price*2;
        }
        else{
            costMap[ road[i].leftCity-1 ][ road[i].rightCity-1 ] = road[i].price;
        }
        if( cityGovNum[ road[i].rightCity-1 ] != road[i].govNum ){
            costMap[ road[i].rightCity-1 ][ road[i].leftCity-1 ] = road[i].price*2;
        }
        else{
            costMap[ road[i].rightCity-1 ][ road[i].leftCity-1 ] = road[i].price;
        }
    }

    //Dijkstra's Algorithm
    int dis[v];
    int sure[v];//確定值
    //待n個城市，只需要做n-1次的更新就好。
    for(i=0;i<n-1;i++){
        //將第"visitOrder[i]"列的距離傳到dis
        for(j=0;j<v;j++){
            dis[j] = costMap[ visitOrder[i]-1 ][j];
        }
        //初始化確定值
        for(j=0;j<v;j++){
            sure[j]=0;
        }
        sure[ visitOrder[i]-1 ]=1;
        for(j=0;j<v;j++){
            if(!sure[j]){
                //找確定值
                int index=0;
                int min = 999;
                for(k=0;k<v;k++){
                    if(!sure[k] && dis[k]<min){
                        index=k;
                        min=dis[k];
                    }
                }
                sure[index]=1;
                //由index延伸
                for(k=0;k<v;k++){
                    if( costMap[index][k]!=999 && costMap[index][k]!=0 ){
                        if( dis[index]+costMap[index][k] < dis[k] ){
                            dis[k] = dis[index]+costMap[index][k];
                        }
                    }
                }
            }
        }
        //將新的dis覆蓋原本的第"visitOrder[i]"列
        for(j=0;j<v;j++){
            costMap[ visitOrder[i]-1 ][j] = dis[j];
        }
    }
    printf("\n");

    //總結過路費
    int total=0;
    for(i=0;i<n-1;i++){
        total += costMap[ visitOrder[i]-1 ][ visitOrder[i+1]-1 ];
    }
    printf("Least Fee is %d\n",total);
}