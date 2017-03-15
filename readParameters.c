#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/stat.h>
/*#include<math.h>*/
#include"forthis.h"

/* Reads basic parameters of the island populations  */

int main(){
  FILE *stream;
  int i,j,nhap,nislands;
  
  fiopen("census","r",stream);
  firead(&nhap,sizeof(int),1,stream);
  firead(&nislands,sizeof(int),1,stream);
  double fit[nhap];
  unsigned int census[nislands][nhap];
  double cc[nislands];
  firead(fit,sizeof(fit),1,stream);
  firead(census,sizeof(unsigned int),nislands*nhap,stream);
  firead(cc,sizeof(cc),1,stream);
  fgetc(stream);

  if(ferror(stream)||!feof(stream)){
    fprintf(stderr,"%s\n","Misformed file");
    exit(1);
  }
  
  printf("# haplotypes: %d\n# islands: %d\nFitnesses:\n",nhap,nislands);
  for(i=0;i<nhap;i++)
    printf(" %d: %f\n",i,fit[i]);
  printf("%s","Island demographics:");
  for(i=0;i<nislands;i++){
    printf("\n Island #%d:\t",i+1);
    for(j=0;j<nhap;j++)
      printf("%u%s",census[i][j],(!((j+1)%16)&(j!=nhap-1))?"\n\t\t":" ");
  }
  printf("\nCarrying capacities:\n");
  for(i=0;i<nislands;++i)
    printf(" Island %d: %f\n",i+1,cc[i]);
  
  fclose(stream);
  return 0;
}

    
  
