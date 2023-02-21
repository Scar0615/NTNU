#include<stdio.h>
#include<string.h>

typedef struct rational{
    int p; //numerator
    int q; //denominator
}list;

int rec;

int gcd(int p,int q){
    int tmp;
    while(p%q!=0){
        tmp=q;
        q=p%q;
        p=tmp;
    }
    return q;
}

void rational_mul( struct rational *r, struct rational r1, struct rational r2){
    r->p=r1.p*r2.p;
    r->q=r1.q*r2.q;
    rec=gcd(r->p,r->q);
    r->p/=rec;
    r->q/=rec;
}
void rational_div( struct rational *r, struct rational r1, struct rational r2){
    r->p=r1.p*r2.q;
    r->q=r1.q*r2.p;
    rec=gcd(r->p,r->q);
    r->p/=rec;
    r->q/=rec;
}

int main(){
    char str[300],op[75];//op=operations
    char *delim="(),+-*/";
    char *take;
    int num[100]={0},co=0,cn=0,i; //co=operations counting & cn=numbers counting
    list f[75];
    list fr,ff; //fr=fraction record & ff=final fractions
    fgets(str,300,stdin);
    for(i=0;i<75;i++){
        f[i].p=0;f[i].q=0;
    }
    for(i=0;i<strlen(str);i++){
        if(str[i]=='+'||str[i]=='-'||str[i]=='*'||str[i]=='/'){
            op[co]=str[i];
            co++;
        }
    }
    take=strtok(str,delim);
    while(take!=NULL){
        for(i=0;i<strlen(take);i++){
            num[cn]*=10;
            num[cn]+=take[i]-'0';
        }
        take=strtok(NULL,delim);
        cn++;
    }
    num[cn-1]=(num[cn-1]+38)/10;
    for(i=0;i<cn/2;i++){
        f[i].p=num[2*i];
        f[i].q=num[2*i+1];
    }
    for(i=0;i<co;i++){
        if(op[i]=='*'||op[i]=='/'){
            if(op[i]=='*'){
                rational_mul(&fr,f[i],f[i+1]);
                f[i].p=fr.p;f[i].q=fr.q;
                f[i+1].p=0;f[i+1].q=0;
                op[i]='+';
            }
            else{
                rational_div(&fr,f[i],f[i+1]);
                f[i].p=fr.p;f[i].q=fr.q;
                f[i+1].p=0;f[i+1].q=0;
                op[i]='+';
            }
        }
    }
    ff.p=f[0].p;ff.q=f[0].q;
    for(i=0;i<co;i++){
        if(op[i]=='+'){
            if(f[i+1].p!=0||f[i+1].q!=0){
                ff.p=ff.p*f[i+1].q+ff.q*f[i+1].p;
                ff.q*=f[i+1].q;
            }
            else{
                ff.p+=0;ff.q+=0;
            }
        }
        else if(op[i]=='-'){
            if(f[i+1].p!=0||f[i+1].q!=0){
                ff.p=ff.p*f[i+1].q-ff.q*f[i+1].p;
                ff.q*=f[i+1].q;
            }
            else{
                ff.p+=0;ff.q+=0;
            }
        }
    }
    rec=gcd(ff.p,ff.q);
    ff.p/=rec;ff.q/=rec;
    printf("(%d,%d)",ff.p,ff.q);
}