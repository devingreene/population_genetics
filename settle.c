#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/stat.h>
#include"forthis.h"

/* `Settle' allows the user to create, populate, and assign fitnesses in */
/* an island model.  With the argument `create', the user is prompted to */
/* enter the number of haplotypes and islands.  With the argument */
/* `populate', one is prompted to enter the island # followed by the */
/* distribution of haplotypes on that island.  With the argument */
/* `fitness', the user is prompted to enter the fitnesses of the */
/* haplotype.  The default fitness is 1.  The default number of */
/* haplotypes on a particular island is 0.  Whether created or modified, */
/* the file has name `census' and stores its data is binary form. */

int main(int argc, char **argv){
  if(argc <2 ){
    fprintf(stderr,"keyword is one of %s, %s, %s, or %s\n",\
	    "create","populate","fitness","cc");
    exit(0);
  }
  
  const double base_fitness=1;  
  const double base_cc=0;
  
  FILE *stream;
  char *lineptr,*endptr;
  int nhap,nislands,island,i;
  size_t dummy; // for getline
  long offset;

  /* to create a file */
  if(!strcmp(argv[1],"create")){ 
    fiopen("census","w",stream);
    nhap=nislands=0;
    while(1){
      printf("%s: ","#haplotypes");
      lineptr = NULL;
      getline(&lineptr,&dummy,stdin);
      if(feof(stdin)) goto end;
      nhap = strtol(lineptr,NULL,0);
      if(nhap>=1)
	break;
    }
    

    while(1){
      printf("%s: ","#islands");
      lineptr=NULL;
      getline(&lineptr,&dummy,stdin);
      /* stop if Control-D */
      if(feof(stdin)) goto end; 
      nislands = strtol(lineptr,NULL,0);
      /* exit loop if number is valid */
      if(nislands>=1)
	break;
    }

    fiwrite(&nhap,sizeof(int),1,stream);
    fiwrite(&nislands,sizeof(int),1,stream);
    
    unsigned *census=(unsigned int*)malloc(nhap*nislands*sizeof(unsigned int));
    

    for(i=0;i<nhap;++i)
      /* default fitness is 1  */
      fiwrite(&base_fitness,sizeof(double),1,stream);
    
    for(i=0;i<nhap*nislands;++i)
      census[i]=0;
    /* default number of haplotypes is zero */
    fiwrite(census,sizeof(unsigned int),nhap*nislands,stream);
    
    for(i=0;i<nislands;++i)
      /* default cc is 0 */
      fiwrite(&base_cc,sizeof(double),1,stream);
  }

  else if(!strcmp(argv[1],"populate")){
    fiopen("census","r+",stream);
    firead(&nhap,sizeof(int),1,stream);
    firead(&nislands,sizeof(int),1,stream);
    do{
    out:
      printf("%s: ","Island #");
      lineptr=NULL;
      getline(&lineptr,&dummy,stdin);
      /* exit if control-D */
      if(feof(stdin)) break;
      island=strtol(lineptr,NULL,0);
      if(island < 1 || island > nislands) goto out;
      printf(" %s:\n ","Enter census of haplotypes in numerical order");
      unsigned int hap[nhap];
      lineptr=NULL;
      getline(&lineptr,&dummy,stdin);
      for(i=0;i<nhap;++i){
	hap[i]=strtol(lineptr,&endptr,0);
	if(lineptr==endptr){
	  fprintf(stderr,"%s\n","Invalid format");
	  goto out;
	}
	lineptr=endptr;
      }
      
      offset=nhap*sizeof(double)+(island-1)*nhap*sizeof(unsigned int);
      fseek(stream,offset,SEEK_CUR);
      fiwrite(hap,sizeof(unsigned int),nhap,stream);
      fseek(stream,-offset-nhap*sizeof(unsigned int),SEEK_CUR);
    } while(!feof(stdin));
    printf("\n");
  }

  else if(!strcmp(argv[1],"fitness")){
    fiopen("census","r+",stream);
    firead(&nhap,sizeof(int),1,stream);
    printf("%s: ","Enter fitnesses of haplotypes in numerical order");
    double fitness[nhap];
    lineptr=NULL;
    getline(&lineptr,&dummy,stdin);
    /* exit if control-d */
    if(feof(stdin)) goto end;
    for(i=0;i<nhap;++i){
      fitness[i]=strtod(lineptr,&endptr);
      if(lineptr==endptr){
	fprintf(stderr,"%s\n","Invalid format");
	goto end;
      }
      lineptr=endptr;
    }
    
    fseek(stream,sizeof(unsigned int),SEEK_CUR);
    fiwrite(fitness,sizeof(fitness),1,stream);
  }
  
  else if(!strcmp(argv[1],"cc")){
    fiopen("census","r+",stream);
    firead(&nhap,sizeof(int),1,stream);
    firead(&nislands,sizeof(int),1,stream);
    /* Seek past demographics and fitnesses */
    printf("%s: ","Enter carrying capacities of islands");
    double cc[nislands];
    lineptr=NULL;
    getline(&lineptr,&dummy,stdin);
    /* exit if control-d */
    if(feof(stdin)) goto end;
    for(i=0;i<nislands;++i){
      cc[i]=strtod(lineptr,&endptr);
      if(lineptr==endptr){
	fprintf(stderr,"%s\n","Invalid format");
	goto end;
      }
      lineptr=endptr;
    }
    fiseek(stream,nhap*nislands*sizeof(int)+nhap*sizeof(double),SEEK_CUR);
    fflush(stream);
    fiwrite(cc,sizeof(cc),1,stream);
  }


  else{
    perror("Invalid argument\n");
    exit(2);
  }

 end:

  fclose(stream);

  return 0;
}
  
  
