#include<stdio.h>
#include<string.h>

typedef struct matrix{
    char name[100];
    int row,column;
    double num[10][10];
}sMatrix;

typedef struct sparse{
    int row,column;
    double num;
}sSparse;

int main(){
    int i,j,functionNum=1,matrixCount=0,findMatrix,index;
    char targetMatrix[100];
    sMatrix m[100];
    // for case 5
    char firstMatrix[100],secondMatrix[100]; 
    int firstIndex,secondIndex,exist,sizeMatch;
    int firstSparseCount,secondSparseCount,sumSparseCount;
    // for case 6
    int square;
    int transposeSparseCount,saveSparseCount;
    int finalSparseIndex;

    while( functionNum>=1 && functionNum<=6 ){
        for(i=0;i<62;i++){
            printf("-");
        }
        printf("\n| Hello, which function do you want to use?                  |\n");
        for(i=0;i<62;i++){
            if(i==0||i==62-1){
                printf("|");
            }
            else{
                printf(" ");
            }
        }
        printf("\n| 1) Enter a matrix M.                                       |\n");
        printf("| 2) Show a matrix M.                                        |\n");
        printf("| 3) Enter the certain rows and columns of matrix M to show. |\n");
        printf("| 4) Show the transpose matrix of M                          |\n");
        printf("| 5) Enter two matrix(M1,M2) and sum.                        |\n");
        printf("| 6) Enter a square matrix M and k, we will show M^k.        |\n");
        for(i=0;i<62;i++){
            printf("-");
        }
        printf("\n\nEnter the number of function(1-6)\n(If your input isn't between 1 and 6, the program will quit.): ");
        scanf("%d",&functionNum);
        switch(functionNum){
            case 1:
            printf("\nEnter the name of matrix: ");
            scanf("%s",m[matrixCount].name);
            printf("Enter the row size and column size of the matrix: ");
            scanf("%d%d",&m[matrixCount].row,&m[matrixCount].column);
            printf("Enter each element of the matrix: ");
            for( i=0 ; i<m[matrixCount].column ; i++ ){
                for( j=0 ; j<m[matrixCount].row ; j++ ){
                    scanf("%lf",&m[matrixCount].num[i][j]);
                }
            }
            printf("\nDone!\n\n");
            matrixCount++;
            break;

            case 2:
            findMatrix=0;
            printf("\nEnter the name of matrix that you want to show: ");
            scanf("%s",targetMatrix);
            //檢查該matrix是否存在
            for( i=0 ; i<matrixCount ; i++ ){
                if( !strcmp( targetMatrix , m[i].name ) ){
                    findMatrix=1;
                    index=i;
                    break;
                }
            }
            //有找到就print，沒找到就error。
            if(findMatrix){
                printf("\nThe element of the matrix \"%s\":\n\n",targetMatrix);
                for( i=0 ; i<m[index].column ; i++ ){
                    for( j=0 ; j<m[index].row ; j++ ){
                        printf("%.2lf ",m[index].num[i][j]);
                    }
                    printf("\n");
                }
                printf("\n");
            }
            else{
                printf("\nThe matrix \"%s\" doesn't exist.\n\n",targetMatrix);
            }
            break;

            case 3:
            findMatrix=0;
            printf("\nEnter the name of matrix that you want to show: ");
            scanf("%s",targetMatrix);
            //檢查該matrix是否存在
            for( i=0 ; i<matrixCount ; i++ ){
                if( !strcmp( targetMatrix , m[i].name ) ){
                    findMatrix=1;
                    index=i;
                    break;
                }
            }
            //有找到就print，沒找到就error。
            if(findMatrix){
                //指定matrix的行編號和列編號
                int rowToShow,columnToShow;
                printf("How many row do you want to show: ");
                scanf("%d",&rowToShow);
                printf("How many column do you want to show: ");
                scanf("%d",&columnToShow);
                int subRow[rowToShow],subCol[columnToShow];
                printf("Enter the row number respectively: ");
                for( i=0 ; i<rowToShow ; i++ ){
                    scanf("%d",&subRow[i]);
                }
                printf("Enter the column number respectively: ");
                for( i=0 ; i<columnToShow ; i++ ){
                    scanf("%d",&subCol[i]);
                }
                printf("\n\nThis is the customized submatrix of matrix M:\n\n");
                //根據使用者指定的row和column印出
                for( i=0 ; i<columnToShow ; i++ ){
                    for( j=0 ; j<rowToShow ; j++ ){
                        printf("%.2lf ",m[index].num[ subCol[i]-1 ][ subRow[j]-1 ]);
                    }
                    printf("\n");
                }
                printf("\n");
            }
            else{
                printf("\nThe matrix \"%s\" doesn't exist.\n\n",targetMatrix);
            }
            break;
            
            case 4:
            findMatrix=0;
            printf("\nEnter the name of matrix that you want to show: ");
            scanf("%s",targetMatrix);
            //檢查該matrix是否存在
            for( i=0 ; i<matrixCount ; i++ ){
                if( !strcmp( targetMatrix , m[i].name ) ){
                    findMatrix=1;
                    index=i;
                    break;
                }
            }
            //有找到就print，沒找到就error。
            if(findMatrix){
                printf("\nThe transpose of the matrix \"%s\":\n\n",targetMatrix);
                for( i=0 ; i<m[index].row ; i++ ){
                    for( j=0 ; j<m[index].column ; j++ ){
                        printf("%.2lf ",m[index].num[j][i]);
                    }
                    printf("\n");
                }
                printf("\n");
            }
            else{
                printf("\nThe matrix \"%s\" doesn't exist.\n\n",targetMatrix);
            }
            break;

            case 5:
            firstSparseCount=0;
            secondSparseCount=0;
            sumSparseCount=0;
            firstIndex=-1;
            secondIndex=-1;
            exist=1;
            sizeMatch=1;
            printf("\nEnter the name of matrix M1:");
            scanf("%s",firstMatrix);
            printf("Enter the name of matrix M2:");
            scanf("%s",secondMatrix);
            //檢查這兩個matrix是否存在
            for( i=0 ; i<matrixCount ; i++ ){
                if( !strcmp( firstMatrix , m[i].name ) ){
                    firstIndex=i;
                }
                if( !strcmp( secondMatrix , m[i].name ) ){
                    secondIndex=i;
                }
            }
            if(firstIndex==-1){
                printf("\nThe matrix \"%s\" doesn't exist.\n\n",firstMatrix);
                exist=0;
            }
            if(secondIndex==-1){
                printf("\nThe matrix \"%s\" doesn't exist.\n\n",secondMatrix);
                exist=0;
            }
            if(!exist){
                break;
            }
            //檢查兩個matrix的row和column是否一樣
            if( m[firstIndex].row != m[secondIndex].row || m[firstIndex].column != m[secondIndex].column ){
                sizeMatch=0;
            }
            if(!sizeMatch){
                printf("The size of two matrices are different.\n");
                break;
            }
            if( exist && sizeMatch ){
                //將兩個matrix轉換成sparse matrix
                int Row = m[firstIndex].row;
                int Col = m[firstIndex].column;
                sSparse firstSparse[ Row * Col ];
                for( i=0 ; i<Col ; i++ ){
                    for( j=0 ; j<Row ; j++ ){
                        if( m[firstIndex].num[i][j] != 0 ){
                            firstSparse[ firstSparseCount ].row = i;
                            firstSparse[ firstSparseCount ].column = j;
                            firstSparse[ firstSparseCount ].num = m[firstIndex].num[i][j];
                            firstSparseCount++;
                        }
                    }
                }
                sSparse secondSparse[ Row * Col ];
                for( i=0 ; i<Col ; i++ ){
                    for( j=0 ; j<Row ; j++ ){
                        if( m[secondIndex].num[i][j] != 0 ){
                            secondSparse[ secondSparseCount ].row = i;
                            secondSparse[ secondSparseCount ].column = j;
                            secondSparse[ secondSparseCount ].num = m[secondIndex].num[i][j];
                            secondSparseCount++;
                        }
                    }
                }
                //將兩個matrix相加
                sSparse sumSparse[ firstSparseCount + secondSparseCount ];
                int firstSparseIndex=0,secondSparseIndex=0,sumSparseIndex=0;
                double temp;
                for( i=0 ; i<Col ; i++ ){
                    for( j=0 ; j<Row ; j++ ){
                        temp=0;
                        if( firstSparse[ firstSparseIndex ].row == i && firstSparse[ firstSparseIndex ].column == j ){
                            temp += firstSparse[ firstSparseIndex ].num;
                            firstSparseIndex++;
                        }
                        if( secondSparse[ secondSparseIndex ].row == i && secondSparse[ secondSparseIndex ].column == j ){
                            temp += secondSparse[ secondSparseIndex ].num;
                            secondSparseIndex++;
                        }
                        if(temp!=0){
                            sumSparse[ sumSparseCount ].row = i;
                            sumSparse[ sumSparseCount ].column = j;
                            sumSparse[ sumSparseCount ].num = temp;
                            sumSparseCount++;
                        }
                    }
                }
                //印出結果
                printf("\n\nThis is the sum of matrix \"%s\" and matrix \"%s\":\n\n",firstMatrix,secondMatrix);
                for( i=0 ; i<Col ; i++ ){
                    for( j=0 ; j<Row ; j++ ){
                        if( sumSparse[ sumSparseIndex ].row == i && sumSparse[ sumSparseIndex ].column == j ){
                            printf("%.2lf ",sumSparse[ sumSparseIndex ].num);
                            sumSparseIndex++;
                        }
                        else{
                            printf("0.00 ");
                        }
                    }
                    printf("\n");
                }
                printf("\n");
            }
            break;

            case 6:
            square=0;
            findMatrix=0;
            transposeSparseCount=0;
            printf("\nEnter the name of matrix that you want to do the multiplication: ");
            scanf("%s",targetMatrix);
            //檢查該matrix是否存在
            for( i=0 ; i<matrixCount ; i++ ){
                if( !strcmp( targetMatrix , m[i].name ) ){
                    findMatrix=1;
                    index=i;
                    break;
                }
            }
            if(findMatrix){
                //檢查該matrix是否為square matrix
                if( m[index].row == m[index].column ){
                    square=1;
                }
                // is square
                if(square){
                    int k;
                    printf("Enter the exponent k: ");
                    scanf("%d",&k);
                    //將該matrix轉換成transpose matrix並以sparse matrix儲存
                    int Row = m[index].row;
                    int Col = m[index].column;
                    sSparse transposeSparse[ Row * Col ],saveSparse[ Row * Col ],finalSparse[ Row * Col ];
                    for( i=0 ; i<Col ; i++ ){
                        for( j=0 ; j<Row ; j++ ){
                            if( m[index].num[i][j] != 0 ){
                                transposeSparse[ transposeSparseCount ].row = j;
                                transposeSparse[ transposeSparseCount ].column = i;
                                transposeSparse[ transposeSparseCount ].num = m[index].num[i][j];
                                transposeSparseCount++;
                            }
                        }
                    }
                    //先讓saveSparse等於(M^1)
                    for( i=0 ; i<transposeSparseCount ; i++ ){
                        saveSparse[i].row = transposeSparse[i].column;
                        saveSparse[i].column = transposeSparse[i].row;
                        saveSparse[i].num = transposeSparse[i].num;
                        saveSparseCount = transposeSparseCount;
                    }
                    int r,c,saveR,tranR;
                    double save[Row],tran[Row],get;
                    for( i=2 ; i<=k ; i++ ){
                        finalSparseIndex=0;
                        //finalSparse = saveSparse * transposeSparse
                        for( r=0 ; r<Row ; r++ ){
                            for( c=0 ; c<Col ; c++ ){
                                //給予初始值
                                get=0;
                                for( saveR=0 ; saveR<Row ; saveR++ ){
                                    save[saveR]=0;
                                    tran[saveR]=0;
                                }
                                //找出欲運算的元素
                                for( saveR=0 ; saveR<saveSparseCount ; saveR++ ){
                                    if( saveSparse[ saveR ].row == r ){
                                        save[ saveSparse[ saveR ].column ] = saveSparse[ saveR ].num;
                                    }
                                }
                                for( tranR=0 ; tranR<transposeSparseCount ; tranR++ ){
                                    if( transposeSparse[ tranR ].row == c ){
                                        tran[ transposeSparse[ tranR ].column ] = transposeSparse[ tranR ].num;
                                    }
                                }
                                //進行乘法運算
                                for( saveR=0 ; saveR<Row ; saveR++ ){
                                    get += save[saveR] * tran[saveR];
                                }
                                //存入finalSparse
                                if(get!=0){
                                    finalSparse[ finalSparseIndex ].row = r;
                                    finalSparse[ finalSparseIndex ].column = c;
                                    finalSparse[ finalSparseIndex ].num = get;
                                    finalSparseIndex++;
                                }
                            }
                        }
                        //將finalSparse存入saveSparse
                        for( j=0 ; j<finalSparseIndex ; j++ ){
                            saveSparse[j].row = finalSparse[j].row;
                            saveSparse[j].column = finalSparse[j].column;
                            saveSparse[j].num = finalSparse[j].num;
                        }
                        saveSparseCount = finalSparseIndex;
                    }
                    //印出結果
                    finalSparseIndex=0;
                    printf("\n\nThis is the result of the matrix %s^%d :\n\n",targetMatrix,k);
                    for( i=0 ; i<Col ; i++ ){
                        for( j=0 ; j<Row ; j++ ){
                            if( finalSparse[ finalSparseIndex ].row == i && finalSparse[ finalSparseIndex ].column == j ){
                                printf("%.2lf ",finalSparse[ finalSparseIndex ].num);
                                finalSparseIndex++;
                            }
                            else{
                                printf("0.00 ");
                            }
                        }
                        printf("\n");
                    }
                    printf("\n");
                }
                // isn't square
                else{
                    printf("\nThe matrix \"%s\" isn't square matrix.\n\n",targetMatrix);
                }
            }
            else{
                printf("\nThe matrix \"%s\" doesn't exist.\n\n",targetMatrix);
            }
            break;

            default:
            printf("\nThanks for using.\n\n");
            break;
        }
    }
    return 0;
}