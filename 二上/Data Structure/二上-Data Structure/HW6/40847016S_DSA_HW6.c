#include<conio.h>
#include<ctype.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int isOperator(char c){
    if( c=='+' || c=='-' || c=='*' || c=='/' ){
        return 1;
    }
    else{
        return 0;
    }
}

int priority(char c){
    if( c=='+' || c=='-' ){
        return 1;
    }
    else if( c=='*' || c=='/' ){
        return 2;
    }
    else{
        return 0;
    }
}

double calculation(double a,double b,char c){
    if( c=='+' ){
        return a+b;
    }
    else if( c=='-' ){
        return a-b;
    }
    else if( c=='*' ){
        return a*b;
    }
    else if( c=='/' ){
        return a/b;
    }
    return 0;
}

int main(){
    char infix[50];
    int i;
    while(1){
        printf("Please enter an infix expression and press enter: \n");
        memset(infix,0,sizeof(infix));
        fgets(infix,sizeof(infix),stdin);
        //check the infix expression legal is legal or not
        int len=strlen(infix);
        int errorInfix=0;
        int leftParenthesis=0,rightParenthesis=0;
        char postfix[20], op[20], stack[20];
        double digit[20],ptovStack[10];
        //Rule 1
        if( len>20 ){
            printf("Error - line contains more characters than allowed.\n");
            errorInfix=1;
        }
        //Rule 2
        for( i=0 ; i<len ; i++ ){
            if( isalpha(infix[i]) ){
                printf("Illegal character\n");
                errorInfix=1;
            }
        }
        for( i=0 ; i<len-1 ; i++ ){
            //Rule 6
            if( infix[i]==')' && isdigit(infix[i+1]) ){
                printf("Right parenthesis followed by an identifier\n");
                errorInfix=1;
            }
            //Rule 7
            if( isOperator(infix[i]) && isOperator(infix[i+1]) ){
                printf("Operator followed by an operator\n");
                errorInfix=1;
            }
            //Rule 8
            if( infix[i]=='(' && isOperator(infix[i+1]) ){
                printf("Left parenthesis followed by an operator\n");
                errorInfix=1;
            }
            //Rule 9
            if( isdigit(infix[i]) && infix[i+1]=='(' ){
                printf("Identifier followed by a left parenthesis\n");
                errorInfix=1;
            }
            //Rule 10
            if( infix[i]==')' && infix[i+1]=='(' ){
                printf("Right parenthesis followed by a left parenthesis\n");
                errorInfix=1;
            }
            //Rule 11
            if( isOperator(infix[i]) && infix[i+1]==')' ){
                printf("Operator followed by a left parenthesis\n");
                errorInfix=1;
            }
            //Rule 12
            if( infix[i]=='(' && infix[i+1]==')' ){
                printf("Left parenthesis followed by a right parenthesis\n");
                errorInfix=1;
            }
        }
        //Rule 4
        if( isOperator(infix[0]) ){
            printf("First character an operator\n");
            errorInfix=1;
        }
        //Rule 5
        if( isOperator(infix[len-2]) ){
            printf("Last character an operator\n");
            errorInfix=1;
        }
        //Rule 13&14
        for( i=0 ; i<len ; i++ ){
            if( infix[i]=='(' ){
                leftParenthesis++;
            }
            if( infix[i]==')' ){
                rightParenthesis++;
            }
        }
        if( leftParenthesis > rightParenthesis ){
            printf("Unmatched left parenthesis\n");
            errorInfix=1;
        }
        else if( rightParenthesis > leftParenthesis ){
            printf("Unmatched right parenthesis\n");
            errorInfix=1;
        }
        //If the infix is legal, then continue, otherwise, re-enter the infix.
        if(errorInfix){
            printf("The infix is invalid\n");
        }
        else{
            int index=0, height=0, postLen=0;
            //initialization
            memset(postfix,0,sizeof(postfix));
            memset(op,0,sizeof(op));
            memset(stack,0,sizeof(stack));
            for( i=0 ; i<20 ; i++ ){
                digit[i]=0;
            }

            //transfer infix to postfix
            for( i=0 ; i<len ; i++ ){
                if( isdigit(infix[i]) ){
                    postfix[postLen++]=infix[i];
                    if( isdigit(infix[i-1]) ){
                        index--;
                        digit[index] = digit[index]*10 + (infix[i]-48);
                        index++;
                    }
                    else{
                        digit[index++]=infix[i]-48;
                    }
                }
                else if( infix[i]=='(' ){
                    stack[++height]=infix[i];
                }
                else if( isOperator(infix[i]) ){
                    while( priority(stack[height]) >= priority(infix[i]) ){
                        postfix[postLen++]=stack[height];
                        op[index++]=stack[height];
                        height--;
                    }
                    stack[++height]=infix[i];
                }
                else if( infix[i]==')' ){
                    while( stack[height]!='(' ){
                        postfix[postLen++]=stack[height--];
                    }
                    height--;
                }
            }
            while(height>0){
                postfix[postLen++]=stack[height];
                op[index++]=stack[height];
                height--;
            }

            int ptvIndex=0;
            //initialization
            for( i=0 ; i<10 ; i++ ){
                ptovStack[i]=0;
            }

            //calculate the result of infix
            for( i=0 ; i<index ; i++ ){
                if( digit[i]!=0 ){
                    ptovStack[ptvIndex++]=digit[i];
                }
                else if( op[i]!=0 ){
                    ptovStack[ptvIndex-2] = calculation( ptovStack[ptvIndex-2], ptovStack[ptvIndex-1], op[i] );
                    ptvIndex--;
                }
            }
            printf("The postfix expression: %s\n",postfix);
            printf("= %.2lf\n",ptovStack[0]);
        }
        printf("Press enter to continue or press esc to quit...\n");
        char keyboardIncident = getch();
        if( keyboardIncident == 27 ){
            printf("Thanks for using!\n");
            break;
        }
        system("cls");
    }
    return 0;
}