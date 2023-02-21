#include <stdio.h>
#include <math.h>
#define PI 3.141592

void rotate(double *,double *,double theta);

int main(){
        double *x,*y,theta;
        printf("Please enter a point:");
        scanf("%lf%lf",&x,&y);
        printf("Please enter theta(0-360):");
        scanf("%lf",&theta);
        rotate(&x,&y,theta);
}

void rotate(double *x,double *y,double theta){
        double xTheta,yTheta,tmpX,tmpY;
        yTheta=( *y/sqrt(pow(*x,2)+pow(*y,2)) ) * cos(theta/180*PI) - ( *x/sqrt(pow(*x,2)+pow(*y,2)) ) * sin(theta/180*PI);
        xTheta=( *x/sqrt(pow(*x,2)+pow(*y,2)) ) * cos(theta/180*PI) + ( *y/sqrt(pow(*x,2)+pow(*y,2)) ) * sin(theta/180*PI);
        tmpX=*x;
        tmpY=*y;
        *x= xTheta * sqrt(pow(tmpX,2)+pow(tmpY,2));
        *y= yTheta * sqrt(pow(tmpX,2)+pow(tmpY,2));
        printf("%lf %lf\n",*x,*y);
}