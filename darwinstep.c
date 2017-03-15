#include<stdlib.h>
#include<stdio.h>

#define Vec ((double(*)[nhap])vec)

/* Routine for carrying out Darwinian step in `iterate'  */

extern void darwinstep(double *vec, const double *fitness,const int nislands,\
				const int nhap);

inline void darwinstep(double *vec, const double *fitness,const int nislands,\
		const int nhap){
  int i,j;
  for(i=0;i<nislands;i++)
    for(j=0;j<nhap;j++)
      Vec[i][j]*=fitness[j];
  return ;
}
