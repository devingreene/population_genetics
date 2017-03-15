#include<stdlib.h>
#include<stdio.h>
#include<string.h>

/* Routine for carrying out migration step in `iterate'  */

extern void migstep(double *vec,const double *mig,const int nislands,const int nhap);
inline void migstep(double *vec,const double *mig,const int nislands,const int nhap){
  int i,j,k;
  double vec2[nislands][nhap];
  for(i=0;i<nislands;++i)
    for(j=0;j<nhap;++j){
      vec2[i][j]=0;
      for(k=0;k<nislands;++k)
	vec2[i][j]+=((double(*)[nislands])mig)[i][k]*	\
	  ((double(*)[nhap])vec)[k][j];
    }
  memcpy(vec,(double*)vec2,sizeof(double)*nislands*nhap);
  return ;
}
