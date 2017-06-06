#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"forthis.h"

#define mut ((double(*)[nhap])Mut)

int main(int argc, char** argv ){
	if(argc==1){
		printf("./makeMut `keyword' ...\n");
		exit(0);
	}

	/* list allowable keywords */
	char *choices[] ={"morgan","flat","symmetric",NULL},**choices0;
	FILE *stream;
	int I=0,i,j,k,n,nhap;
	double A,B,C,D,E,*Mut;

	/* make index alias for keyword */
	for(choices0=choices;*choices0!=NULL;choices0++){
		if(!strcmp(argv[1],*choices0)) break;
		I++;
	}

	if(*choices0==NULL){
		fprintf(stderr,"%s: ","keywords");
		for(choices0=choices;*choices0!=NULL;choices0++,I--)
			fprintf(stderr,"%s%s",*choices0,I == 1 ? "\n" : ", ");
		exit(1);
	}

	if(I==0){ //morgan
		if(argc < 4){
			fprintf(stderr,"%s `nloci' `rate'\n","`morgan'");
			exit(0);
		}
		const int nloci=strtol(argv[2],NULL,0);
		const double rate=strtod(argv[3],NULL);
		nhap = (1<<nloci);
		Mut=calloc(nhap*nhap,sizeof(double));

		/* assign matrix entries */
		for(i=0;i<nhap;++i){
			for(j=i;j<nhap;++j){
				A=1;
				k=i^j;
				for(n=0;n<nloci;++n){
					A *= ((k&1) ? rate : 1-rate);
					k =(k>>1);
				}
				mut[i][j]=A;
			}

			for(j=0;j<i;++j)
				mut[i][j]=mut[j][i];
		}
	}

	/* flat */
	else if(I==1||I==2){ //`flat' or `symmetric'
		if(argc != 4){
			fprintf(stderr,"`%s' `nhap' `default_rate'\n",choices[I]);
			exit(0);
		}
		nhap=strtol(argv[2],NULL,0);
		if(nhap<=0){
			fprintf(stderr,"%s\n","Improper argument #2");
			exit(1);
		}
		Mut=calloc(nhap*nhap,sizeof(double));
		double default_rate=strtod(argv[3],NULL);
		char *line=NULL,*ep,*string[3];
		size_t n;

		/* assign matrix entries */
		/* default rate */
		for(i=0;i<nhap;++i)
			for(j=0;j<nhap;++j)
				if(i!=j) mut[i][j]=default_rate;
				else {
					mut[i][j]=1-(nhap-1)*default_rate;
					if(mut[i][j]<0){
						fprintf(stderr,"%s\n","Diagonal elements cannot be negative!");
						exit(1);
					}
				}

		while(1){
entry: // from row sum check below
			printf("%s: ","Ordered pair");
			getline(&line,&n,stdin);
			if(feof(stdin)){
				printf("\n");
				break;
			}
			string[0]=strsep(&line,", \t:;");
			string[1]=strsep(&line,", \t:;");
			string[2]=strsep(&line,", \t:;");
			i=strtol(string[0],&ep,0);
			j=strtol(string[1],&ep,0);
			A=strtod(string[2],&ep);
			if(i<0 || i>=nhap || j<0 || j>=nhap || i==j || string[2]==ep) {
				fprintf(stderr,"%s\n","Invalid entry");
				continue;
			}
			/* store in case of bad entries */
			B=mut[j][i];
			C=mut[i][i];
			if(I==2){ // keyword=symmetric
				D=mut[i][j];
				E=mut[j][j];
				mut[i][j]=A;
				/* columns *must* add to one! */
				mut[j][j]+=D-A;
			}
			mut[j][i]=A;
			mut[i][i]+=B-A;
			if(mut[i][i] < 0 || mut[j][j] < 0){
				fprintf(stderr,"%s\n","Diagonal elements cannot be negative!");
				/* revert */
				mut[j][i]=B;
				mut[i][i]=C;
				if(I==2){ //keyword=symmetric
					mut[i][j]=D;
					mut[j][j]=E;
				}
				goto entry;
			}
			/* Confirm change */
			fprintf(stderr,"%d%s->%d: %e\n",i,((I==2)?"<":""),j,A);
		}
	}
	else exit(1);


	/* Write results to a file */
	fiopen("mut","w",stream);
	fiwrite(Mut,sizeof(double),nhap*nhap,stream);
	if(fclose(stream)!=0){
		fprintf(stderr,"%s\n","Closing file");
		exit(1);
	}

	return 0;
}
