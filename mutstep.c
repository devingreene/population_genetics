#include<stdlib.h>
#include<stdio.h>
#include<string.h>

/* routine for carrying out mutation step in `iterate' */

extern void mutstep(double *vec,const double *mut,const int nislands,const int nhap);

inline void mutstep(double *vec,const double *mut,const int nislands,const int nhap){
	int i,j,k;
	double vec2[nislands][nhap];
	for(i=0;i<nislands;i++)
		for(j=0;j<nhap;j++){
			vec2[i][j]=0;
			for(k=0;k<nhap;k++)
				vec2[i][j] += ((double(*)[nhap])mut)[j][k]*((double(*)[nhap])vec)[i][k];
		}
	memcpy(vec,(double *)vec2,sizeof(double)*nislands*nhap);
	return ;
}

