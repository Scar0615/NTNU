#include<stdio.h>

typedef struct _sMatrix{
    int x;
    int y;
    int value;
    struct _sMatrix *pNext;
} sMatrix;

void set_matrix( sMatrix *m, int a, int b, int val );
// m[a][b] = val
void free_matrix( sMatrix *m );
void print_matrix( sMatrix *m );
int add_matrix( sMatrix *output , sMatrix a, sMatrix b );
// output = a + b
// return -1 if fail
int mul_matrix( sMatrix *output , sMatrix a, sMatrix b );
// output = a * b
// return -1 if fail
void transpose_matrix( sMatrix *m );
int det_matrix( sMatrix *m ); // Calculate the determinant.
