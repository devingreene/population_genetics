#include<stdlib.h>
#include<stdio.h>
#include<gsl/gsl_randist.h>
#include<unistd.h>
#include<math.h>
#include"forthis.h"

#define max(a,b) a>b?a:b

extern gsl_rng * R;
extern void darwinstep(double *vec, const double *fitness,const int nislands \
		       ,const int nhap);
extern void mutstep(double *vec,const double *mut,const int nislands,	\
		    const int nhap);
extern void recomstep(double *vec,const double *recom,const int nislands,\
	       const int nhap,const unsigned int *N);
extern void migstep(double *vec,const double *mig,const int nislands,const int nhap);

extern void iterateCC(const int nislands,const int nhap,			\
	     unsigned int vec[nislands][nhap],double *mut,double *recom, \
	     double *mig,const double *fitness,const unsigned char ifrecom,\
	       const unsigned char ifmig,const double *cc){
  int i,j;

  /* take a census for each islands to sample from later */
  unsigned int N[nislands];
  double fN[nislands]; // to use at end
  double vecf[nislands][nhap];
  for(i=0;i<nislands;++i){
    N[i]=0;
    for(j=0;j<nhap;++j)
      N[i]+=vec[i][j];
  }

  /* convert to float */
  for(i=0;i<nislands;++i)
    for(j=0;j<nhap;++j)
      vecf[i][j]=(double)vec[i][j];

  /* intra-islands */
  /* Darwin! */
  darwinstep((double*)vecf,fitness,nislands,nhap);
  /* mutation */
  mutstep((double*)vecf,mut,nislands,nhap);
  /* recombination */
  if(ifrecom)
    recomstep((double*)vecf,recom,nislands,nhap,N);
    
  /* `inter-island migration' */
  /* if migration ...  */
  if(ifmig)
    migstep((double*)vecf,mig,nislands,nhap);

  /* "Genetic drift" */
  for(i=0;i<nislands;++i){
    fN[i]=0;
    for(j=0;j<nhap;++j)
      fN[i]+=vecf[i][j];
    N[i]=(unsigned int)(fN[i]*exp(1-fN[i]/cc[i])+0.5);
    gsl_ran_multinomial(R,nhap,N[i],((double(*)[nhap])vecf)[i],	\
			((unsigned int(*)[nhap])vec)[i]);
  }
  return ;
}
