#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"forthis.h"

/* Creates the data file `mig' containing the parameters for inter-island */
/* migration.  There are three keywords: */

/* `array': Must be followed dim1 dim2.  Forms a dim1 by dim2 array of */
/* islands.  Adjacent islands (where diagonal neighbors are considered */
/* adjacent) exchange migrants at equal rates. */

/* `torus': Like array, but migration "wraps around" */

/* `flat': Starting with a set number of islands and a default migration */
/* 	rate between any pair of islands, program then prompts user to */
/* 	enter whatever migration rates for whatever pair of islands */
/* 	she see fit.  Note that this is asymmetric.  The rate `1->2' */
/* 	may differ from `2->1' */

/* `symmetric': Like `flat', but migration rates are constrained to be */
/* 	     symmetric between islands in a pair. */


int main(int argc,char** argv){
  if(argc==1){
    printf("./makeMig `keyword' ...\n");
    exit(0);
  }

  FILE *stream;
  double *mig,rate,A,B,C,D;
  size_t sizeofmig;
  int I=0,i,j,m,n;
  size_t N;
  char *choices[]={"array","flat","symmetric","torus",NULL},**choices0;
  for(choices0=choices;*choices0!=NULL;choices0++){
    if(!strcmp(*choices0,argv[1])) break;
    I++;
  }

  if(*choices0==NULL){
    fprintf(stderr,"%s: ","Keywords are");
    for(choices0=choices;*choices0!=NULL;choices0++)
      fprintf(stderr,"%s`%s'",(choices0==choices)?"":", ",*choices0);
    fprintf(stderr,"\n");
    exit(1);
  }
      
  if(I==0 || I==3){ //array || torus
    if(argc != 5){
      fprintf(stderr,"`%s' `dim1' `dim2' `rate'\n",choices[I]);    
      exit(1);
    }
  
    const int x=strtol(argv[2],NULL,0);
    const int y=strtol(argv[3],NULL,0);
    rate=strtod(argv[4],NULL);
    mig = malloc(sizeof(double)*x*y*x*y);
    double(*Mig)[y][x][y] = (double(*)[y][x][y])mig;

  
    if(x<=0 || y<=0)
      Error("Dimensions must be postive");
    if(x>y)
      Error("Number of rows in island array cannot exceed \
number of columns");

    /* set all equal to 0 initially */
    for(i=0;i<x;++i)
      for(j=0;j<y;++j)
	for(m=0;m<x;++m)
	  for(n=0;n<y;++n) Mig[i][j][m][n]=0;


    if(I==0){
      if(x==1 && y==1) Mig[0][0][0][0] = 1;
      else if(x==1){
	/* ends */
	Mig[0][0][0][0]=Mig[0][y-1][0][y-1]=1-rate;
	Mig[0][1][0][0]=Mig[0][y-2][0][y-1]=rate;
      
	/* middle */
	for(i=1;i<y-1;++i){
	Mig[0][i][0][i]=1-2*rate;
	Mig[0][i-1][0][i]=rate;
	Mig[0][i+1][0][i]=rate;
	}
      }
  
      if(x > 1 && y > 1){  
	/* corners */
	Mig[0][0][0][0]=1-3*rate;
	Mig[1][0][0][0]=Mig[0][1][0][0]=Mig[1][1][0][0]=rate;
	Mig[0][y-1][0][y-1]=1-3*rate;
	Mig[1][y-1][0][y-1]=Mig[0][y-2][0][y-1]=Mig[1][y-2][0][y-1]=rate;
	Mig[x-1][0][x-1][0]=1-3*rate;
	Mig[x-2][0][x-1][0]=Mig[x-1][1][x-1][0]=Mig[x-2][1][x-1][0]=rate;
	Mig[x-1][y-1][x-1][y-1]=1-3*rate;
	Mig[x-2][y-1][x-1][y-1]=Mig[x-1][y-2][x-1][y-1]=	\
	  Mig[x-2][y-2][x-1][y-1]=rate;
  
	/* edges */
	for(i=1;i<x-1;++i){
	  Mig[i][0][i][0]=1-5*rate;
	  Mig[i-1][0][i][0]=Mig[i+1][0][i][0]=\
	    Mig[i-1][1][i][0]=Mig[i][1][i][0]=Mig[i+1][1][i][0]=rate;
	  Mig[i][y-1][i][y-1]=1-5*rate;
	  Mig[i-1][y-1][i][y-1]=Mig[i+1][y-1][i][y-1]=\
	    Mig[i-1][1][i][y-1]=Mig[i][1][i][y-1]=Mig[i+1][1][i][y-1]=rate;
	}
	for(i=1;i<y-1;++i){
	  Mig[0][i][0][i]=1-5*rate;
	  Mig[0][i-1][0][i]=Mig[0][i+1][0][i]=\
	    Mig[1][i-1][0][i]=Mig[1][i][0][i]=Mig[1][i+1][0][i]=rate;
	  Mig[x-1][i][x-1][i]=1-5*rate;
	  Mig[x-1][i-1][x-1][i]=Mig[x-1][i+1][x-1][i]=\
	    Mig[1][i-1][x-1][i]=Mig[1][i][x-1][i]=Mig[1][i+1][x-1][i]=rate;
	}
  
	/* interior */
	for(i=1;i<x-1;++i)
	  for(j=1;j<y-1;++j){
	    Mig[i][j][i][j]=1-8*rate;
	    for(m=-1;m<=1;++m)
	      for(n=-1;n<=1;++n){
		if(!(m|n)) continue;
		Mig[i+m][j+n][i][j]=rate;
	      }
	  }
      }
    }
    else{ //torus
      if(x==1 && y==1) Mig[0][0][0][0]=1;
      else if(x==1 && y==2)
	for(i=0;i<y;++i){
	  Mig[0][0][0][0]=Mig[0][1][0][1]=1-rate;
	  Mig[0][1][0][0]=Mig[0][0][0][1]=rate;
	}
      else if(x==1 && y>2)
	for(j=0;j<y;++j){
	  Mig[0][j][0][j]=1-2*rate;
	  Mig[0][(j+1)%y][0][j]=rate;
	  Mig[0][(j+y-1)%y][0][j]=rate;
	}
      
      else if(x==2 && y==2)
	for(i=0;i<2;++i)
	  for(j=0;j<2;++j){
	    Mig[i][j][i][j]=1-3*rate;
	    Mig[1-i][j][i][j]=Mig[1-i][1-j][i][j]=\
	      Mig[i][1-j][i][j]=1-3*rate;
	  }

      else if(x==2)
	for(i=0;i<y;++i){
	  Mig[0][i][0][i]=1-5*rate;
	  Mig[1][i][0][i]=rate;
	  Mig[0][(i+y-1)%y][0][i]=rate;
	  Mig[1][(i+y-1)%y][0][i]=rate;
	  Mig[0][(i+1)%y][0][i]=rate;
	  Mig[1][(i+1)%y][0][i]=rate;
	  Mig[1][i][1][i]=1-5*rate;
	  Mig[0][i][1][i]=rate;
	  Mig[1][(i+y-1)%y][1][i]=rate;
	  Mig[0][(i+y-1)%y][1][i]=rate;
	  Mig[1][(i+1)%y][1][i]=rate;
	  Mig[0][(i+1)%y][1][i]=rate;
	}
	  
      else{ // x>2 && y>2
	for(i=0;i<x;++i)
	  for(j=0;j<y;++j){
	    Mig[i][j][i][j]=1-8*rate;
	    for(m=-1;m<=1;++m)
	      for(n=-1;n<=1;++n)
		if(m!=0 || n!=0)
		  Mig[(i+y+m)%y][(j+y+n)%y][i][j]=rate;
	  }
      }
    }	
  sizeofmig = x*y*x*y*sizeof(double);
  }
  
  else if(I==1 || I==2){//flat or symmetric
    if(argc!=4){
      fprintf(stderr,"`%s' `number of islands' `default rate'\n",choices[I]);
      exit(0);
    }

    const int nislands=strtol(argv[2],NULL,0);
    if(nislands<=0)
      Error("Number of islands must be postive");
    const double default_rate=strtod(argv[3],NULL);
    if(default_rate < 0)
      Error("Default rate must be non-negative");

    mig = (double*)malloc(sizeof(double)*nislands*nislands);
    double (*Mig)[nislands] = (double(*)[nislands])mig;

    /* enter the default rates */
    for(i=0;i<nislands;++i){
      Mig[i][i]=1;
      for(j=0;j<nislands;++j){
	if(i!=j){ 
	  Mig[i][j]=default_rate;
	  Mig[j][j]-=default_rate;
	}
      }
    }
    if(Mig[0][0]<0)
      Error("Default rate too large");
        
    char *line=NULL,*ep,*string[3];
    /* enter the non default rates as standard input */
    while(1){
    entry:
      printf("%s: ","Enter islands pair followed by rate");
      getline(&line,&N,stdin);
      if(feof(stdin)) {
	printf("\n");
	break;
      }
      string[0]=strsep(&line," ,;:\t");
      j=strtol(string[0],NULL,0);
      if(j<=0){
      	fprintf(stderr,"%s\n","Invalid input");
	continue; 
      }
      string[1]=strsep(&line," ,;:\t");
      i=strtol(string[1],NULL,0);
      if(i<=0 || i==j){
	fprintf(stderr,"%s\n","Invalid input");
	continue; 
      }	
      string[2]=strsep(&line," ,;:\t");
      rate=strtod(string[2],&ep);
      if(string[2]==ep){
	fprintf(stderr,"%s\n","Invalid input");
	continue;
      }
      i--;j--;
      A=Mig[i][j];
      B=Mig[j][j];
      Mig[i][j]=rate;
      Mig[j][j]+=A-rate;
      if(I==2){
	C=Mig[j][i];
	D=Mig[i][i];
	Mig[j][i]=rate;
	Mig[i][i]+=C-rate;
      }
      if(Mig[i][i]<0 || Mig[j][j]<0){
	/*revert*/
	Mig[i][j] = A;
	Mig[j][j]=B;
	if(I==2){
	  Mig[j][i]=C;
	  Mig[i][i]=D;
	}
	fprintf(stderr,"%s\n","Rate entered is too large");
	goto entry;
      }
      
      /* print out confirmation */
      fprintf(stderr,"%d%s->%d: %e\n",j+1,((I==2)?"<":""),i+1,rate);
    }
    sizeofmig=nislands*nislands*sizeof(double);
  }
  else exit(1); // this shouldn't happen

  /* write to file */
  fiopen("mig","w",stream);
  fiwrite(mig,sizeofmig,1,stream);
  fclose(stream);

  return 0;
}
