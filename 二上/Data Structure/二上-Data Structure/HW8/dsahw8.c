#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

typedef struct bstnode{
    char number[5];
    int price;
    int amount;
    int numToInt; //將number轉成int儲存
    struct bstnode *left;
    struct bstnode *right;
}BSTNode;

BSTNode *insertNode(BSTNode *tree, BSTNode *unit){
    BSTNode *current;
    BSTNode *parent;

    unit->left = NULL;
    unit->right = NULL;

    //如果樹是空的
    if( tree == NULL ){
        return unit;
    }
    else{
        current = tree;

        //尋找該unit在整個tree裡面的定位
        while( current != NULL ){
            parent = current;
            //編號小的往左排
            if( unit->numToInt < current->numToInt ){
                current = current->left;
            }
            //編號大的往右排
            else if( unit->numToInt > current->numToInt ){
                current = current->right;
            }
        }

        //插入unit
        if(unit->numToInt > parent->numToInt){
            parent->right = unit;
        }
        else{
            parent->left = unit;
        }
    }
    return tree;
}

BSTNode *findNode(BSTNode *ptr, int num){
    //從ptr指到的節點開始找
    while(ptr != NULL){
        //找到就回傳
        if(num == ptr->numToInt){
            return ptr;
        }
        //如果目前節點的numToInt比unit大，向左邊子節點移動。
        else if(num < ptr->numToInt){
            ptr = ptr->left;
        }
        //否則向右邊子節點移動
        else{
            ptr = ptr->right;
        }
    }
    printf("\nSorry, we don't find the product you want. :(\n");
    return 0; //找不到
}

BSTNode *findParent(BSTNode *ptr, int value, int *pos){
    BSTNode *parent;
    //從ptr開始找
    parent = ptr;
    *pos = 0;
    while(ptr != NULL){
        //找到目標：回傳此節點之父節點
        if(ptr->numToInt == value){
            return parent;
        }          
        else{
            parent = ptr;
            //往左找
            if(ptr->numToInt > value){
                *pos = -1;
                ptr = ptr->left; 
            }
            //往右找
            else{
                *pos = 1;
                ptr = ptr->right;
            }
        }
    }
    return 0; //找不到
}

BSTNode *deleteNode(BSTNode *tree, int value){
    BSTNode *parent;
    BSTNode *ptr;
    BSTNode *next;
    int pos;

    parent = findParent(tree, value, &pos); //從root開始找value之父節點
    if(parent != NULL){
        //判斷目前節點在父節點左邊或右邊
        switch(pos){
            case -1:
                ptr = parent->left;
                break;
            case 1:
                ptr = parent->right;
                break;
            case 0:
                ptr = parent;
                break;
        }
        //節點沒有左子樹
        if(ptr->left == NULL){
            //如果要刪的是根節點
            if(parent == ptr){
                tree = tree->right;
            }
            else{
                //要刪除的節點在父節點右方，所以將父節點的右節點指向刪除節點的右節點。
                if(pos == 1){
                    parent->right = ptr->right;
                }
                //要刪除的節點在父節點左方，所以將父節點的左節點指向刪除節點的右節點。
                else{
                    parent->left = ptr->right;
                }
            }
            free(ptr);
        }
        //節點沒有右子樹
        else if(ptr->right == NULL){
            //如果要刪的是根節點
            if(parent == ptr){
                tree = tree->left;
            }
            else{
                //要刪除的節點在父節點右方，所以將父節點的右節點指向刪除節點的左節點。
                if(pos == 1){
                    parent->right = ptr->left;
                }
                //要刪除的節點在父節點左方，所以將父節點的左節點指向刪除節點的左節點。
                else{
                    parent->left = ptr->left;
                }
            }
            free(ptr);
        }
        //節點有左右子樹
        else{
            parent = ptr;
            next = ptr->left; //找取代點next, 從ptr左邊開始找
            //往左子節點之右子樹找最大值當取代點
            while(next->right != NULL){
                parent = next; //parent為next之父節點
                next = next->right;
            }
            //用ptr取代next
            memset( ptr->number, 0, sizeof(ptr->number) );
            strcpy(ptr->number, next->number);
            ptr->price = next->price;
            ptr->amount = next->amount;
            ptr->numToInt = next->numToInt;
            //繞過next節點
            parent->right = next->left;

            free(next);
        }
    }
    return tree;
}

void inorder(BSTNode *ptr){
    if(ptr){
        inorder(ptr->left);
        printf("Number: %s | Price: %d | Amount: %d\n",ptr->number ,ptr->price ,ptr->amount);
        inorder(ptr->right);
    }
}

int maxPrice = 0;

//找到最高價
void findHighestPrice(BSTNode *ptr){
    if(ptr){
        findHighestPrice(ptr->left);
        printf("Number: %s | Price: %d\n",ptr->numToInt ,ptr->price);
        if(ptr->price > maxPrice){
            maxPrice = ptr->price;
        }
        findHighestPrice(ptr->right);
    }
}

//印出價格為最高價的product
void printHPProduct(BSTNode *ptr){
    if(ptr){
        printHPProduct(ptr->left);
        if(ptr->price == maxPrice){
            printf("Most valued product: %s\n",ptr->number);
        }
        printHPProduct(ptr->right);
    }
}

void inorderFileOutput(BSTNode *ptr,FILE *file){
    if(ptr){
        inorderFileOutput(ptr->left,file);
        fprintf(file, "%s %d %d\n",ptr->number,ptr->price,ptr->amount);
        inorderFileOutput(ptr->right,file);
    }
}

int main(){
    FILE *txt = NULL;
    FILE *out = NULL;
    char fileName[30],newFileName[30];
    int functionNum = 1;
    int a=0;

    //檢查檔案是否存在
    printf("Please enter the file name of the product: ");
    if(fgets(fileName,sizeof(fileName),stdin)==NULL){
        printf("Error file name input!\n");
        return 0;
    }
    if(fileName[strlen(fileName)-1]=='\n'){
        fileName[strlen(fileName)-1]=0;
    }
    else{
        while( (a=fgetc(stdin) )!='\n' && a!=EOF ){}
    }
    if( ( txt=fopen(fileName,"r") )==NULL ){
        printf( "File could not be opened!\n" );
        return 0;
    }

    system("clear");

    //讀取資訊並儲存在BST
    BSTNode *productTree = NULL;
    BSTNode *findProduct = NULL;
    BSTNode *nextProduct = NULL;
    BSTNode *deleteProduct = NULL;
    BSTNode *exportProduct = NULL;
    BSTNode *importProduct = NULL;
    BSTNode *modifyProduct = NULL;
    BSTNode *unit;
    char info[50];
    char *part[3]; //[0]:num [1]:price [2]:amount
    char *word;
    char searchNum[5],modify[5];
    int partIndex, i;
    int num; //給findNode函式使用
    int amountToExport,amountToImport;
    int newPrice;
    while(!feof(txt)){
        fgets(info, sizeof(info), txt);
        if(info[strlen(info)-1]=='\n'){
            info[strlen(info)-1]=0;
        }
        partIndex = 0;
        word = strtok(info, " ");
        while( word != NULL ){
            part[partIndex]=word;
            word = strtok(NULL, " ");
            partIndex++;
        }
        unit = (BSTNode *)malloc(sizeof(BSTNode) );
        strcpy( unit->number, part[0] );
        unit->price = atoi(part[1]);
        unit->amount = atoi(part[2]);
        unit->numToInt = atoi(part[0]);
        productTree = insertNode(productTree, unit);
    }

    while( functionNum>=1 && functionNum<=8 ){
        for(i=0;i<81;i++){
            printf("-");
        }
        printf("\n| Hello, which function do you want to use?                                     |\n");
        for(i=0;i<81;i++){
            if(i==0||i==81-1){
                printf("|");
            }
            else{
                printf(" ");
            }
        }
        printf("\n| 1) Search for a product by the product number.                                |\n");
        printf("| 2) Inport products.                                                           |\n");
        printf("| 3) Delete a product.                                                          |\n");
        printf("| 4) Export products.                                                           |\n");
        printf("| 5) List all product information in increasing order of product number.        |\n");
        printf("| 6) Modify the product number of a product.                                    |\n");
        printf("| 7) Search for a product with the highest price and output its product number. |\n");
        printf("| 8) Save the information to file and quit.                                     |\n");
        for(i=0;i<81;i++){
            printf("-");
        }
        printf("\n\nEnter the number of function(1~8): ");
        scanf("%d",&functionNum);
        getchar();
        switch( functionNum ){
            /*功能1：搜尋*/
            case 1:
            memset( searchNum, 0, sizeof(searchNum) );

            //輸入要找的product number
            printf("Please enter the number of product that you want to search: ");
            fgets(searchNum,sizeof(searchNum),stdin); 
            if(searchNum[strlen(searchNum)-1]=='\n'){
                searchNum[strlen(searchNum)-1]=0;
            }

            num = atoi(searchNum);

            findProduct = findNode(productTree, num);
            //找到就印出所有資訊
            if( findProduct ){
                printf("\nThis is the information of the product: \n\n");
                printf("Number: %s\n",findProduct->number);
                printf("Price: %d\n",findProduct->price);
                printf("Amount: %d\n\n",findProduct->amount);
            }
            //沒找到就印出error message
            printf("\n");
            break;

            /*功能2：進口*/
            case 2:
            printf("Please enter the number of product that you want to import: ");
            fgets(searchNum,sizeof(searchNum),stdin); 
            if(searchNum[strlen(searchNum)-1]=='\n'){
                searchNum[strlen(searchNum)-1]=0;
            }

            printf("Please enter the amount that you want to import: ");
            scanf("%d",&amountToImport);

            num = atoi(searchNum);
            importProduct = findNode(productTree, num);
            //如果已經存在
            if( importProduct ){
                printf("\n%d of product #%s are imported. Amount: %d -> %d\n\n",amountToImport ,importProduct->number ,importProduct->amount ,importProduct->amount + amountToImport);
                importProduct->amount += amountToImport;
            }
            //如果不存在
            else{
                printf("But we will insert #%s into product list, so please enter the price of it: ",searchNum);
                scanf("%d",&newPrice);
                importProduct = (BSTNode *)malloc( sizeof(BSTNode) );
                strcpy(importProduct->number, searchNum);
                importProduct->amount = amountToImport;
                importProduct->price = newPrice;
                importProduct->numToInt = atoi(searchNum);
                productTree = insertNode(productTree, importProduct);
                printf("\n%d of product #%s are imported.\n\n",amountToImport ,importProduct->number);
            }
            break;
            
            /*功能3：刪除*/
            case 3:
            memset( searchNum, 0, sizeof(searchNum) );

            //輸入要刪除的product number
            printf("Please enter the number of product that you want to delete: ");
            fgets(searchNum,sizeof(searchNum),stdin); 
            if(searchNum[strlen(searchNum)-1]=='\n'){
                searchNum[strlen(searchNum)-1]=0;
            }

            num = atoi(searchNum);
            deleteProduct = findNode(productTree, num);
            if( deleteProduct ){
                productTree = deleteNode(productTree, num);
                printf("\nThe product #%s has been deleted.\n\n",searchNum);
            }
            else{
                printf("\n");
            }
            break;

            /*功能4：出口*/
            case 4:
            memset( searchNum, 0, sizeof(searchNum) );

            //輸入要export的product number
            printf("Please enter the number of product that you want to export: ");
            fgets(searchNum,sizeof(searchNum),stdin); 
            if(searchNum[strlen(searchNum)-1]=='\n'){
                searchNum[strlen(searchNum)-1]=0;
            }

            num = atoi(searchNum);
            exportProduct = findNode(productTree, num);

            printf("Please enter the amount that you want to export: ");
            scanf("%d",&amountToExport);

            //amount不夠就印出error message
            if(exportProduct->amount < amountToExport){
                printf("\n%d is bigger than the amount of the product #%s: %d\n",amountToExport ,exportProduct->number ,exportProduct->amount);
                printf("The amount of the product is not enough to export.\n");
            }
            else{
                printf("\n%d of product #%s are exported. Amount: %d -> %d\n\n",amountToExport ,exportProduct->number ,exportProduct->amount ,exportProduct->amount - amountToExport);
                exportProduct->amount -= amountToExport;
            }
            break;
            
            /*功能5：列出所有產品*/
            case 5:
            printf("\nHere are all information of products: \n\n");
            inorder(productTree);
            sleep(4);
            break;

            /*功能6：修改產品號碼*/
            case 6:
            //輸入要修改的product number
            printf("Please enter the number of product that you want to modify: ");
            fgets(searchNum,sizeof(searchNum),stdin); 
            if(searchNum[strlen(searchNum)-1]=='\n'){
                searchNum[strlen(searchNum)-1]=0;
            }

            num = atoi(searchNum);

            modifyProduct = findNode(productTree, num);
            //找到就開始修改
            if( modifyProduct ){
                //輸入新號碼
                printf("Please enter the new number of the product: ");
                fgets(modify,sizeof(modify),stdin); 
                if(modify[strlen(modify)-1]=='\n'){
                    modify[strlen(modify)-1]=0;
                }

                //覆蓋舊號碼
                strcpy(modifyProduct->number, modify);
                modifyProduct->numToInt = atoi(modify);
            }
            printf("\nThe product has changed the number from #%s to #%s.\n\n",searchNum, modify);
            break;

            /*功能7：找出價格最高的產品*/
            case 7:
            printf("\n");
            findHighestPrice(productTree);
            printf("\n");
            printHPProduct(productTree);
            sleep(3);
            break;

            /*功能8：輸入檔名並輸出成檔案。*/
            default:
            //input輸出檔案的檔名
            printf("Please enter the name of the file you want to output: ");
            if(fgets(newFileName,sizeof(newFileName),stdin)==NULL){
                printf("File name error!\n");
                return 0;
            }
            if(newFileName[strlen(newFileName)-1]=='\n'){
                newFileName[strlen(newFileName)-1]=0;
            }
            if( ( out=fopen(newFileName,"w") )==NULL ){
                printf( "File could not be opened!\n" );
                return 0;
            }

            //輸出檔案
            inorderFileOutput(productTree, out);
            fclose(txt);
            fclose(out);

            printf("\nThanks for using.\n");
            return 0;
            break;
        }
        sleep(2);
        system("clear");
    }
    return 0;
}