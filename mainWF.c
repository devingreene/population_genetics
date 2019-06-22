#include<stdlib.h>
#include<stdio.h>

#ifndef INFINITE
#include<gsl/gsl_rng.h>
#endif

#include<fcntl.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<math.h>
#include"forthis.h"

#define FIX_THRES 0.99

#ifdef INFINITE
#define NTYPE double
#else
#define NTYPE unsigned int
#endif

#define minprop(vec,olen,ilen)  double min = INFINITY;\
	NTYPE sum;\
	int i,j;\
	for(i=0;i<(olen);i++){\
		sum = 0;\
		for(j=0;j<(ilen);j++){\
			sum += (vec)[i][j] ;\
		}\
		double prop = (double)((vec)[i][nhap-1])/sum;\
		min = (prop < min)?prop:min;\
	}

#ifndef INFINITE
gsl_rng * R;
#endif
void iterateWF(const int nislands,const int nhap,			
		NTYPE vec[nislands][nhap],double *mut,double *recom, 
		double *mig,const double *fitness,const unsigned char ifrecom,
		const unsigned char ifmig);

int main(int argc, char **argv){
	if(argc<3 || argc>5){
		printf("./exec ngen nsim mperiod=<num> --fixrate\n");
		return 0;
	}

	/* some declarations */
	char *string=malloc(sizeof(char)*32),*back_string;
	const int ngen = strtol(argv[1],NULL,0),nsim = strtol(argv[2],NULL,0);
	int mperiod,nhap,nislands,ntypes,i,j,k,fd[4];
	unsigned long int seed;
	FILE *censusstream,*mutstream,*recomstream,*migstream;
	double ifrecom;
	struct stat size[4];
#ifndef INFINITE
	R = gsl_rng_alloc(gsl_rng_mt19937);
#endif

	/* If mperiod=<num> argument is present, process this */
	if(argc>3){
		strcpy(string,argv[3]);
		back_string=strsep(&string,"=");
		if(strcmp(back_string,"mperiod")){
			fprintf(stderr,"%s\n",					\
					"Third argument must have form `mperiod=<num>'");
			exit(1);
		}
		mperiod=strtol(string,NULL,0);
		if(mperiod<=0){
			fprintf(stderr,"%s\n","Migration period must be a positive integer");
			exit(1);
		}
	}
	else mperiod = 1;

	/* Get some random bits to make a seed for gsl */

	int fd_0=open("/dev/urandom",O_RDONLY);
	read(fd_0,(void*)&seed,sizeof(seed));
	close(fd_0);
#ifndef INFINITE
	gsl_rng_set(R,seed);
#endif

	/* Open files and start by recording their sizes */

#define err_msg(file) fprintf(stderr,"File \"%s\" doesn't exist\n",(file))
	if((fd[0]=open("census",O_RDONLY)) < 0)
    {
        err_msg("census");
        exit(1);
    }
	if((fd[1]=open("mut",O_RDONLY)) < 0)
    {
        err_msg("mut");
        exit(1);
    }
	if((fd[2]=open("recom",O_RDONLY)) < 0)
    {
        err_msg("recom");
        exit(1);
    }
	if((fd[3]=open("mig",O_RDONLY)) < 0)
    {
        err_msg("mig");
        exit(1);
    }

	for(i=0;i<4;i++){
		fstat(fd[i],&size[i]);
	}

	/* Read from `census' file */
	censusstream=fdopen(fd[0],"r");
	firead(&nhap,sizeof(int),1,censusstream);
	firead(&nislands,sizeof(int),1,censusstream);

	ntypes=nhap*nislands;

	/* With nhap and nislands loaded, check that "parameter" files have */
	/*   correct size */
	if(size[0].st_size != (off_t)(2*sizeof(int)+ntypes*sizeof(unsigned int)+ \
				nhap*sizeof(double)+nislands*sizeof(double))){
		fprintf(stderr,"%s\n","`census' file has wrong size");
		exit(2);
	}
	if(size[1].st_size != (off_t)(nhap*nhap*sizeof(double))){
		fprintf(stderr,"%s\n","`mut' file has wrong size");
		exit(2);
	}
	if(size[2].st_size != (off_t)((1+nhap*nhap*(nhap-1)/2)*sizeof(double))){
		fprintf(stderr,"%s\n","`recom' file has wrong size");
		exit(2);
	}
	if(nislands != 1&& size[3].st_size != (off_t)(nislands*nislands*sizeof(double))){
		fprintf(stderr,"%s\n","`mig' file has wrong size");
		exit(2);
	}

	/* Read data files to set up parameters */
	unsigned int initial[nislands][nhap];
	NTYPE vec[nislands][nhap];
	double fitness[nhap];

	firead(fitness,sizeof(fitness),1,censusstream);
	firead(initial,sizeof(initial),1,censusstream);
	fclose(censusstream);

	double mut[nhap][nhap];
	double recom[nhap*nhap*(nhap-1)/2];
	double mig[nislands][nislands];

	/* Load mutation matrix */
	mutstream = fdopen(fd[1],"r");
	firead(mut,sizeof(double),nhap*nhap,mutstream);
	fclose(mutstream);

	/* Load recombination rate and matrix */
	recomstream = fdopen(fd[2],"r");
	firead(&ifrecom,sizeof(double),1,recomstream);
	firead(recom,sizeof(double),nhap*nhap*(nhap-1)/2,recomstream);
	fclose(recomstream);

	/* Load migration matrix */
	if(nislands!=1){
		migstream = fdopen(fd[3],"r");
		firead(mig,sizeof(double),nislands*nislands,migstream);
		fclose(migstream);
	}

	/* Now everything's loaded.  Begin simulation. */

	int fixrate = 0;
	unsigned fixcount;
	if (argc > 4){
		if(strcmp(argv[4],"--fixrate")){
			fprintf(stderr,"%s\n","Invalid 4th arg");
			exit(1);
		}
		fixrate = 1;
		fixcount = 0;
	}

	if(fixrate){
		fprintf(stderr,"%s: ","Progress Meter");
		fflush(stdout);
	}

	for(i=0;i<nsim;++i){
        
#ifdef INFINITE
        {
            int i,j;
            for(i = 0 ; i < nislands ; ++i)
                for(j = 0 ; j < nhap ; ++j)
                    vec[i][j] = (NTYPE)initial[i][j];
        }
#else
		memcpy(vec,initial,nhap*nislands*sizeof(int));
#endif

        int report = (nsim < 100)?10:(10*(nsim/100));
		if(fixrate && !((i+1) % report)){
			fprintf(stderr,"%u%s",i+1,(nsim-i-1>=report)?", ":"\n");
			fflush(stderr);
		}

		for(j=0;j<ngen;++j){
			iterateWF(nislands,nhap,vec,(double*)mut,(double*)recom,	\
					(double*)mig,(double*)fitness,ifrecom!=0,	\
					(nislands != 1) & !(j % mperiod));
			if(fixrate){
				minprop(vec,nislands,nhap);
				if(min > FIX_THRES){
					fixcount += 1;
					break;
				}
			}
		}
		if(!fixrate){
			printf("Simulation %d:\n",i+1);
			for(j=0;j<nislands;++j){
				for(k=0;k<nhap;++k)
#ifdef INFINITE
					printf("%.2f ",vec[j][k]);
#else
					printf("%u ",vec[j][k]);
#endif
				printf("\n");
			}
			fflush(stdout);
		}
	}

	if(fixrate){
		printf("Fixation rate: %.3f\n",(double)fixcount/nsim);
		return 0;
	}

	return 0;
}
