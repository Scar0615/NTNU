#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

int main(){
	int32_t cn[5],suit[5],r[5],change,flower;
	bool isFlush=true,isStraight=true;
	printf("Please enter 5 cards(1-52):");
    	for(int32_t a=0; a<5; a++){ 
        	scanf("%d",&cn[a]);
		if(cn[a]<1 || cn[a]>52){
			printf("Error Input!\n");
			return 0;
		}
		suit[a]=cn[a]/13;
		r[a]=cn[a]%13;
		if(r[a]%13==0){
			suit[a]-=1;
			r[a]+=13;
		}
    	}
	//arrangement
	for(int32_t b=0; b<5; b++){
		for(int32_t c=0; c<5; c++){
			if(r[b]<r[c]){
				change = r[c];
				r[c] = r[b];
				r[b] = change;
			}
		}
 	}
	flower=suit[0];
	//judgment
	for(int d=1;d<5;d++){
		if(suit[d]!=flower){
			isFlush=false;
		}
	}
	for(int e=0;e<4;e++){
		if(r[e]+1 != r[e+1]){
			isStraight=false;
		}
	}
	if(r[0]==1 && isStraight==false){
		isStraight=true;
		for(int32_t g=1;g<5;g++){
			if(r[g] != g+9){
				isStraight=false;
			}
		}
	}
	if(isFlush==true && isStraight==true){
		printf("Straight flush\n");
	}
	else if(isFlush==true){
		printf("flush\n");
	}
	else if(isStraight==true){
		printf("Straight\n");
	}
	else if((r[0]==r[1] && r[2]==r[3] && r[1]==r[2]) || (r[0]==r[1] && r[3]==r[4] && r[1]==r[3]) || (r[1]==r[2] && r[3]==r[4] && r[2]==r[3])){
		printf("Four of a kind\n");
	}
	else if((r[0]==r[1] && r[1]==r[2] && r[3]==r[4] && r[2]!=r[3]) ||
		(r[0]==r[1] && r[2]==r[3] && r[3]==r[4] && r[1]!=r[2])){
	    printf("Full house\n");
	}
	else if((r[0]==r[1] && r[1]==r[2] && r[3]!=r[4] && r[2]!=r[3]) ||
		(r[0]!=r[1] && r[2]==r[3] && r[1]==r[2] && r[3]!=r[4] && r[0]!=r[4]) ||
		(r[2]==r[3] && r[3]==r[4] && r[0]!=r[1] && r[1]!=r[2])){
	    printf("Three of a kind\n");
	}
	else if((r[0]==r[1] && r[2]==r[3] && r[1]!=r[2] && r[0]!=r[4] && r[2]!=r[4]) ||
		 (r[0]==r[1] && r[3]==r[4] && r[1]!=r[3] && r[0]!=r[2] && r[3]!=r[2]) ||
		 (r[1]==r[2] && r[3]==r[4] && r[2]!=r[3] && r[0]!=r[1] && r[0]!=r[3])){
		printf("Two pair\n");
	}
	else if(r[0]==r[1] || r[1]==r[2] || r[2]==r[3] || r[3]==r[4] || r[4]==r[5]){
		printf("One pair\n");
	}
	else{
		printf("High card\n");
	}
}
