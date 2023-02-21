#include<stdlib.h>
#include"fin03.h"

void set_matrix( sMatrix *m, int a, int b, int val ){
    sMatrix *pMatrix = calloc( 1,sizeof(sMatrix) );
    if(pMatrix==NULL){
        printf( "%s(%d) %s: NULL pointer!\n", __FILE__, __LINE__, __FUNCTION__ );
        return;
    }
    pMatrix -> x = a;
    pMatrix -> y = b;
    pMatrix -> value = val;
    pMatrix -> pNext = NULL;
}

void free_matrix( sMatrix *m ){
    if(m==NULL){
        printf( "%s(%d) %s: NULL pointer!\n", __FILE__, __LINE__, __FUNCTION__ );
        return;
    }
    free(m);
}

void print_matrix( sMatrix *m ){
    if(m==NULL){
        printf( "%s(%d) %s: NULL pointer!\n", __FILE__, __LINE__, __FUNCTION__ );
        return;
    }
    printf("(%d,%d,%d)\n",m->x,m->y,m->value);
}

int add_matrix( sMatrix *output , sMatrix a, sMatrix b ){
    if(&a==NULL){
        printf( "%s(%d) %s: NULL pointer!\n", __FILE__, __LINE__, __FUNCTION__ );
        return -1;
    }
    if(&b==NULL){
        printf( "%s(%d) %s: NULL pointer!\n", __FILE__, __LINE__, __FUNCTION__ );
        return -1;
    }
    output -> x = a.x + b.x;
    output -> y = a.y + b.y;
    output -> value = a.value + b.value;
    return output;
}

int mul_matrix( sMatrix *output , sMatrix a, sMatrix b ){
    output -> x = a.x * b.x;
    output -> y = a.y * b.y;
    output -> value = a.value * b.value;
    return output;
}

void transpose_matrix( sMatrix *m ){
    printf("Sorry I don't know how to implement...\n");
}

int det_matrix( sMatrix *m ){
    printf("Sorry I don't know how to implement...\n");
}