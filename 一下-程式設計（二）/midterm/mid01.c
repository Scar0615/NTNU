#include<stdio.h>
#include<string.h>

typedef struct rational{
    int p; //numerator
    int q; //denominator
}sValue;

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

void arrange(char *str){
	for(int i=0; i<strlen(str); i++){
		if(str[i]==' '){
			for(int j=i; j<200; j++){
				str[j]=str[j+1];
			}
		}
	}
}

int main(){
    char *list="(), ";
    char *token;
    char str[500]={0};
    char op[20]={0};
    int i,oc=0,nc=0,sm; //o=operation n=number sm=1 means <0
    int num[60]={0};
    sValue f[20]={0};
    sValue fr,ff;
    fgets(str,sizeof(str),stdin);
    arrange(str);
    token=strtok(str,list);
    while(token!=NULL){
        sm=0;
        if( strcmp(token,"+")==0 ){op[oc]='+';oc++;}
        else if( strcmp(token,"-")==0 ){op[oc]='-';oc++;}
        else if( strcmp(token,"*")==0 ){op[oc]='*';oc++;}
        else if( strcmp(token,"/")==0 ){op[oc]='/';oc++;}
        else{
            num[nc]=atoi(token);
            nc++;
        }
        token=strtok(NULL,list);
    }
    for(i=2;i<nc-1;i+=3){
        if(num[i]==0){
            printf("Invalid input.\n");
            return 0;
        }
    }
    for(i=0;i<nc-1;i+=3){
        f[i/3].p=num[i]*num[i+2]+num[i+1];
        f[i/3].q=num[i+1];
    }
    for(i=0;i<oc;i++){
        if(op[i]=='*'){
            rational_mul(&fr,f[i],f[i+1]);
            f[i].p=fr.p;f[i].q=fr.q;
            f[i+1].p=0;f[i+1].q=0;
            op[i]='+';
        }
        else if(op[i]=='/'){
            rational_div(&fr,f[i],f[i+1]);
            f[i].p=fr.p;f[i].q=fr.q;
            f[i+1].p=0;f[i+1].q=0;
            op[i]='+';
        }
    }
    ff.p=f[0].p;ff.q=f[0].q;
    for(i=0;i<oc;i++){
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
    int a,b,c;
    a=ff.p/ff.q;
    b=ff.p%ff.q;
    c=ff.q;
    printf("(%d,%d,%d)\n",a,b,c);
}