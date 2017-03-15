#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/stat.h>
#include<math.h>
#include"forthis.h"

/* Reads the entries of the `mut' matrix */

int main(){
  FILE *stream;
  off_t fsize;
  int i,j,e,dim;
  double sq;
  /* set stat structure to 0 */
  struct stat *buf =calloc(sizeof(struct stat),1);
  stat("mut",buf);
  fsize=buf->st_size;
  /* Check file integrity */
  sq=sqrt(fsize/8);
    if(fsize==0 || !!(fsize % 8) || sq!=floor(sq)){
      fprintf(stderr,"%s\n","File `mut' non-existant or misread");
      exit(1);
    }

  /* `dim'=dimension of matrix */
  dim=(int)sq;

  printf("#Haplotypes: %d\n",dim);
  fiopen("mut","r",stream);
  double mut[dim][dim];

  e=fread(mut,sizeof(double),dim*dim,stream);
  if(e < dim*dim || ferror(stream)){
    fprintf(stderr,"%s\n","Problems in reading `mut', file possibly malformed");
    exit(1);
  }
  
  fclose(stream);

  for(i=0;i<dim;i++)
    for(j=i+1;j<dim;j++){
      if(mut[i][j]!=mut[j][i]){
	printf("%d->%d: %e\n",i,j,mut[j][i]);
	printf("%d->%d: %e\n",j,i,mut[i][j]);
      }
      else 
	printf("%d<->%d: %e\n",i,j,mut[j][i]);
    }
  
	

  return 0;
}
