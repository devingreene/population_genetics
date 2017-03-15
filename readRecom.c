/* Reads the entries of the `recom' matrix */

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/stat.h>
#include<math.h>
#include"forthis.h"

int main(){
  FILE *stream;
  off_t fsize;
  int i,j,k,dim,S=0;
  double rate;
  /* set stat structure to 0 */
  struct stat *buf =calloc(sizeof(struct stat),1);
  stat("recom",buf);
  fsize=buf->st_size;
  /* Check file integrity.  `dim' corresponds to `nhap' */
  for(dim=1;(S+1)*8<fsize;++dim)
    S=dim*dim*(dim-1)/2;
  if((S+1)*(int)sizeof(double)!=fsize)
    Error("`recom' is improperly formatted");
  /* else S is number of entries in matrix */
  dim--;

  printf("#Haplotypes: %d\n",dim);
  fiopen("recom","r",stream);
  double *recom = calloc(S,sizeof(double));

  firead(&rate,sizeof(double),1,stream);
  firead(recom,sizeof(double),S,stream);
  
  fclose(stream);
  printf("Rate: %e\n",rate);
  
  for(k=0;k<dim;k++)
    for(i=0;i<dim;i++)
      for(j=i+1;j<dim;j++)
	printf("%d-x-%d->%d: %e\n",i,j,k,*recom++);

  return 0;
}
