#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct P{int i,b,r,a,s,f,t,w,q; struct P *n;}P;

P* m(int i,int b){P* p=(P*)malloc(sizeof(P)); p->i=i; p->b=b; p->r=b; p->a=0; p->s=-1; p->f=0; p->t=0; p->w=0; p->q=1; p->n=NULL; return p;}

void e(P** h,P** t,P* p){p->n=NULL; if(!*h){*h=*t=p; return;} (*t)->n=p; *t=p;}

void g(P** h,P** t,P* p){ if(!*h){*h=*t=p; p->n=NULL; return;} p->n=*h; *h=p; }

P* d(P** h,P** t){ if(!h) return NULL; P p=*h; *h=(*h)->n; if(!*h) *t=NULL; p->n=NULL; return p;}

int main(){
    int n;
    if(scanf("%d",&n)!=1) return 0;
    P* a[n];
    for(int i=0;i<n;i++){ int id,bt; scanf("%d %d",&id,&bt); a[i]=m(id,bt); }

    P *h1=NULL,*t1=NULL,*h2=NULL,*t2=NULL,*h3=NULL,*t3=NULL;
    for(int i=0;i<n;i++) e(&h1,&t1,a[i]);

    int c=0;
    int q=1;
    int S=8,T=16,U=32;
    int V=2,W=4;

    while(h1||h2||h3){
        int fnd=0;
        for(int k=0;k<3;k++){
            int idx = ((q-1)+k)%3 + 1;
            if((idx==1 && h1) || (idx==2 && h2) || (idx==3 && h3)){ q=idx; fnd=1; break; }
        }
        if(!fnd) break;
        int rem = (q==1? S : q==2? T : U);
        while(rem>0){
            P* x = (q==1? d(&h1,&t1) : q==2? d(&h2,&t2) : d(&h3,&t3));
            if(!x) break;
            if(x->s==-1) x->s=c;
            int gq = (q==1? V : q==2? W : INT_MAX/2);
            if(gq>rem) gq=rem;
            if(x->r<gq) gq=x->r;
            x->r -= gq;
            c += gq;
            rem -= gq;
            if(x->r==0){
                x->f = c;
            } else {
                if(gq == (q==1? V : q==2? W : INT_MAX/2) && q!=3){
                    x->q = (x->q==1)?2:3;
                    if(x->q==2) e(&h2,&t2,x); else e(&h3,&t3,x);
                } else {
                    if(q==1) g(&h1,&t1,x);
                    else if(q==2) g(&h2,&t2,x);
                    else g(&h3,&t3,x);
                }
            }
            if(rem==0) break;
        }
        q = q%3 + 1;
    }

    double A=0,B=0,Cv=0;
    for(int i=0;i<n;i++){
        P* x=a[i];
        x->t = x->f - x->a;
        x->w = x->t - x->b;
        A += x->t; B += x->w; Cv += (x->s - x->a);
        printf("%d %d %d %d %d\n", x->i, x->b, x->t, x->w, x->s - x->a);
    }
    printf("Average Turnaround Time = %.2f\n", A/n);
    printf("Average Waiting Time = %.2f\n", B/n);
    printf("Average Response Time = %.2f\n", Cv/n);
    return 0;
}