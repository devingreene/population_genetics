#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"forthis.h"

int main(int argc,char** argv){
  if(argc==1){
    printf("./makeRecom `keyword' ...\n");
    exit(0);
  }
  if(argc > 1 && strncmp(argv[1],"morgan",sizeof("morgan")))
    Error("Only keyword `morgan' accepted at this point\n");

  if(argc < 4){
    fprintf(stderr,"%s\n","`morgan'  `nloci' `recombination rate'\
 [`linkage'\\ \ncoefficients ... (there should be nloci-1 coefficients)]");
    exit(0);
  }
  
  const int nloci=strtol(argv[2],NULL,0); 
  if (nloci > 6){
	fprintf(stderr,"%s\n","Number of loci should not exceed 6");
	exit(1);
  }
  const int nhap = (1<<nloci); //2**nloci
  /* Eventual number of entries in matrix */
  const int S=nhap*nhap*(nhap-1)/2;
  const double rate=strtod(argv[3],NULL);
  double link[nloci-1],A,a,recom[nhap][nhap][nhap],\
    *recom2=(double*)calloc(S,sizeof(double));
  int i,i0,j,j0,k,k0,m,n,n0,b,linkage;
  FILE *stream;
  
  if(argc>4){
    if(strcmp(argv[4],"linkage"))
      Error("Trailing keyword must be `linkage'") 
      else linkage=1;
    if(argc < 4+nloci)
      Error("Insufficient number of linkage parameters");
    for(i=0;i<nloci-1;++i){
      link[i]=strtod(argv[5+i],NULL);
      if(link[i]<0 || link[i]>0.5)
	Error("Linkage parameters must be between `0' and `0.5'");
    }
  }
  else linkage=0; // which case no linkage

  if(!linkage)
    for(i=0;i<nloci-1;++i)
      link[i]=0.5; // no linkage

  /*  For this procedure we consider `i' to be "Dad" and `j' to be */
  /* "Mom".  The first locus always comes from Dad.  We represent a */
  /* possible crossover event as a bigstring of length nloci-1, where */
  /* `1' implies crossover.  We then check if `k' could have come */
  /*  from the parents and crossover event */
  for(i=0;i<nhap;++i){
    for(j=0;j<nhap;++j){
      if(i==j) continue;
      for(k=0;k<nhap;++k){
	if((k&1)^(i&1)){ 
	  /* Iterate if first locus of child not Dad's */
	  recom[k][i][j]=0;
	  continue; 
	}
	A=0;
	for(n=0;n<nhap/2;++n){
	  /* Shift to next locus to start replication process.  
	     `b' is parent currently being replicated */
	  i0=i>>1;j0=j>>1;k0=k>>1;n0=n;a=1;b=i0; 
	  for(m=0;m<nloci-1;++m){
	    /* if `1' then cross to other parent, and multiplicatively 
	       accumulate probability */
	    if(n0&1){
	      b=(b==i0)?j0:i0;
	      a*=link[m];
	    }
	    else
	      a*=(1-link[m]);

	    /* Identical locus at `k'?  If not, then stop for this `k' */
	    if((b&1)^(k0&1)){
	      a=0;
	      goto end;
	    }
	    /* else shift to next locus */
	    i0=i0>>1;j0=j0>>1;k0=k0>>1;b=b>>1;n0=n0>>1;
	  }
	  A+=a;
	end:;
	}
	/* Account for proportion `rate' of population having sex :-) */
	A*=rate;
	A+=(1-rate)*(k==i);
	recom[k][i][j]=A;
      }
    }
  }
  
  /* "Compactify" */
  for(i=0;i<nhap;++i)
    for(j=0;j<nhap;++j)
      for(k=j+1;k<nhap;++k)
	*recom2++ = recom[i][j][k] + recom[i][k][j];
  recom2-=S;
  
  /* write resulting matrix to file `recom'*/
  fiopen("recom","w",stream);
  fiwrite(&rate,sizeof(double),1,stream);
  fiwrite(recom2,sizeof(double),S,stream);
  fclose(stream);
  return 0;
}
