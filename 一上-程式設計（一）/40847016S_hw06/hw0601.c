#include<stdio.h>
#include<stdint.h>
#include"hw0601.h"

int (*func)(uint8_t *);
int func01(uint8_t *);
int func02(uint8_t *);
int func03(uint8_t * );
void sort_card(uint8_t *player,int (*func)(uint8_t *));
void print_card(uint8_t *);

uint8_t card[52] = {45, 26, 10, 43, 19, 51, 50, 16, 36, 31, 20, 38, 41, 37, 34, 7, 13, 12, 27, 5, 48, 46, 42, 1, 6, 21, 17, 28, 8, 15, 2, 33, 4, 3, 22, 29, 9, 11, 49, 40, 30, 35, 24, 39, 44, 23, 52, 14, 47, 32, 18, 25};
uint8_t player1[13] = {0};
uint8_t player2[13] = {0};
uint8_t player3[13] = {0};
uint8_t player4[13] = {0};
uint8_t tmp,cardNcount=0;

int main(){
	for(size_t i=0;i<52;i++){
                for(size_t j=0;j<52;j++){
                        if((card[j]-1)==cardNcount){
                                cardNcount++;
                                continue;
                        }
                }
        }
        if(cardNcount!=52){
                printf("Please don't cheat.\n");
                return 0;
        }
        shuffle( player1 , player2 , player3 , player4 );
        printf( "Before:\n" );
        print_card( player1 );
        print_card( player2 );
        print_card( player3 );
        print_card( player4 );
        printf( "Type 01:\n" );
        sort_card( player1 , func01 );
        sort_card( player2 , func01 );
        sort_card( player3 , func01 );
        sort_card( player4 , func01 );
        print_card( player1 );
        print_card( player2 );
        print_card( player3 );
        print_card( player4 );
        printf( "Type 02:\n" );
        sort_card( player1 , func02 );
        sort_card( player2 , func02 );
        sort_card( player3 , func02 );
        sort_card( player4 , func02 );
        print_card( player1 );
        print_card( player2 );
        print_card( player3 );
        print_card( player4 );
        printf( "Type 03:\n" );
        sort_card( player1 , func03 );
        sort_card( player2 , func03 );
        sort_card( player3 , func03 );
        sort_card( player4 , func03 );
        print_card( player1 );
        print_card( player2 );
        print_card( player3 );
        print_card( player4 );
}

void shuffle(uint8_t *player1, uint8_t *player2, uint8_t *player3, uint8_t *player4){
        for(int i = 0; i < 13; i++)
        {
                player1[i] = card[i];
        }
        for(int i = 0; i < 13; i++)
        {
                player2[i] = card[i + 13];
        }
        for(int i = 0; i < 13; i++)
        {
                player3[i] = card[i + 26];
        }
        for(int i = 0; i < 13; i++)
        {
                player4[i] = card[i + 39];
        }
}

void print_card(uint8_t *player){
        if(player == player1){
                for(int i=0;i<13;i++){
                        printf("%u ",player1[i]);
                }
        }
        if(player == player2){
                for(int i=0;i<13;i++){
                        printf("%u ",player2[i]);
                }
        }
        if(player == player3){
                for(int i=0;i<13;i++){
                        printf("%u ",player3[i]);
                }
        }
        if(player == player4){
                for(int i=0;i<13;i++){
                        printf("%u ",player4[i]);
                }
        }
        printf("\n");
}

int func01(uint8_t *card){
        for(size_t i=0;i<13;i++){
                for(size_t j=0;j<13;j++){
                        if((card[j]-1)%13>(card[i]-1)%13){
                                tmp=card[i];
                                card[i]=card[j];
                                card[j]=tmp;
                        }
                }
        }
}

int func02(uint8_t *card){
        uint8_t countT=0;
        for(size_t i=0;i<13;i++){
                for(size_t j=0;j<13;j++){
                        if((card[j]-1)%13>(card[i]-1)%13){
                                tmp=card[i];
                                card[i]=card[j];
                                card[j]=tmp;
                        }
                }
        }
        for(size_t i=0;i<13;i++){
                if(card[i]%13==1 || card[i]%13==2){
                        countT++;
                }
        }
        for(size_t i=0;i<countT;i++){
                for(size_t j=0;j<13;j++){
                        if(card[j]%13==2){
                                tmp=card[j];
                                for(size_t k=j;k<12;k++){
                                        card[k]=card[k+1];
                                }
                                card[12]=tmp;
                                break;
                        }
                }
        }
        for(size_t i=0;i<countT;i++){
                for(size_t j=0;j<13;j++){
                        if(card[j]%13==1){
                                tmp=card[j];
                                for(size_t k=j;k<12;k++){
                                        card[k]=card[k+1];
                                }
                                card[12]=tmp;
                                break;
                        }
                }
        }
}

int func03(uint8_t *card){
        uint8_t tmp;
        for(size_t i=0;i<13;i++){
                for(size_t j=0;j<13;j++){
                        if(card[j]>card[i]){
                                tmp=card[i];
                                card[i]=card[j];
                                card[j]=tmp;
                        }
                }
        }
}

void sort_card(uint8_t *player,int (*func)(uint8_t *)){
        if(player == player1){
                if(func == func01){
                        func01(player1);
                }
                else if(func == func02){
                        func02(player1);
                }
                else if(func == func03){
                        func03(player1);
                }
        }
        if(player == player2){
                if(func == func01){
                        func01(player2);
                }
                else if(func == func02){
                        func02(player2);
                }
                else if(func == func03){
                        func03(player2);
                }
        }
        if(player == player3){
                if(func == func01){
                        func01(player3);
                }
                else if(func == func02){
                        func02(player3);
                }
                else if(func == func03){
                        func03(player3);
                }
        }
        if(player == player4){
                if(func == func01){
                        func01(player4);
                }
                else if(func == func02){
                        func02(player4);
                }
                else if(func == func03){
                        func03(player4);
                }
        }
}
