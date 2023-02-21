#include"rational.h"

int rec,state;

int gcd(int32_t p,int32_t q){
    int tmp;
    while(p%q!=0){
        tmp=q;
        q=p%q;
        p=tmp;
    }
    return q;
}

int rational_set(struct rational *r, int32_t p, int32_t q){
    if(r->q==0){
        return -1;
    }
    else{
        r->p=p;
        r->q=q;
        return 0;
    }
}

int rational_print(const struct rational r){
    printf("(%d,%d)\n",r.p,r.q);
}

void rational_add( struct rational *r, const struct rational r1, const struct rational r2){
    r->p=(r1.p*r2.q)+(r2.p*r1.q);
    r->q=r1.q*r2.q;
    rec=gcd(r->p,r->q);
    r->p/=rec;
    r->q/=rec;
}

void rational_sub( struct rational *r, const struct rational r1, const struct rational r2){
    r->p=(r1.p*r2.q)-(r2.p*r1.q);
    r->q=r1.q*r2.q;
    rec=gcd(r->p,r->q);
    r->p/=rec;
    r->q/=rec;
}
void rational_mul( struct rational *r, const struct rational r1, const struct rational r2){
    r->p=r1.p*r2.p;
    r->q=r1.q*r2.q;
    rec=gcd(r->p,r->q);
    r->p/=rec;
    r->q/=rec;
}
void rational_div( struct rational *r, const struct rational r1, const struct rational r2){
    r->p=r1.p*r2.q;
    r->q=r1.q*r2.p;
    rec=gcd(r->p,r->q);
    r->p/=rec;
    r->q/=rec;
}