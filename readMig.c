/* Reads the entries of the `mig' matrix */

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/stat.h>
#include<math.h>
#include"forthis.h"

int main(){
  FILE *stream;
  off_t fsize;
  int i,j,dim;
  double sq;
  /* set stat structure to 0 */
  struct stat *buf =calloc(sizeof(struct stat),1);
  stat("mig",buf);
  fsize=buf->st_size;
  /* Check file integrity */
  sq=sqrt(fsize/8);
    if(fsize==0 || !!(fsize % 8) || sq!=floor(sq)){
      fprintf(stderr,"%s\n","File `mig' non-existant or misread");
      exit(1);
    }

  /* `dim'=dimension of matrix */
  dim=(int)sq;

  printf("#Islands: %d\n",dim);
  fiopen("mig","r",stream);
  double mig[dim][dim];

  firead(mig,sizeof(double),dim*dim,stream);
  fclose(stream);
  
  for(i=0;i<dim;i++)
    for(j=i+1;j<dim;j++){
      if(mig[i][j]!=mig[j][i]){
	printf("%d->%d: %e\n",i+1,j+1,mig[j][i]);
	printf("%d->%d: %e\n",j+1,i+1,mig[i][j]);
      }
      else 
	printf("%d<->%d: %e\n",i+1,j+1,mig[j][i]);
    }

  return 0;
}
