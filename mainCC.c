#include<stdlib.h>
#include<stdio.h>
#include<gsl/gsl_rng.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include"forthis.h"

/* This is a basic population genetic model which takes into account */
/* fitness differentials, migration, mutation, and recombination.  Each */
/* simulation begins from an initial state consisting of a set of islands */
/* with migration rates between them; a set of haplotypes on each island, */
/* with mutation, recombination, and fitness differentials taken into account. */
/* Each island population is effectively subject to a Wright-Fisher */
/* process, in addition to changes in the haplotype distribution due to */
/* migrations to and from other islands.  The parameters used in the */
/* simulations are read from the following data files: */

/* census: Contains initial haplotype distribution on each island */
/* 	`("census") as well as fitnesses of each haplotypes */
/* mut: 	Mutational rates amongst haplotypes */
/* recom: 	Recombination rates amongst haplotypes */
/* mig: 	Migration rates amongst islands   */

/* Note that migration may be set to be a periodic phenomenon via the */
/* argument `mperiod=<num>' (see code for details).  The argument is */
/* optional.  If not present, it will default to '1`, and migration will */
/* occur every generation. */

gsl_rng * R;
void iterateCC(const int nislands,const int nhap,			\
		unsigned int vec[nislands][nhap],double *mut,double *recom, \
		double *mig,const double *fitness,const unsigned char ifrecom,\
		const unsigned char ifmig,const double *cc);

int main(int argc, char **argv){
	if(argc<3 || argc>4){
		printf("./exec `ngen' `nsim' `mperiod=<num>`\n");
		return 0;
	}

	/* some declarations */
	char *string=malloc(sizeof(char)*32);
	char *string2;
	const int ngen = strtol(argv[1],NULL,0);
	const int nsim = strtol(argv[2],NULL,0);
	int mperiod;
	unsigned long int seed;
	FILE *censusstream,*mutstream,*recomstream,*migstream,*rand_hist;
	int nhap,nislands,ntypes,i,j,k,fd[4];
	double ifrecom;
	struct stat size[4];
	R = gsl_rng_alloc(gsl_rng_mt19937);

	/* If mperiod=<num> argument is present, process this */
	if(argc==4){
		if(strlen(argv[3])>32){
			fprintf(stderr,"%s\n","Third argument too long");
			exit(1);
		}
		strcpy(string,argv[3]);
		string2=strsep(&string,"=");
		if(strcmp(string2,"mperiod")){
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
	/* else make it 0 until nislands verified*/
	else mperiod=0;

	/* Get some random bits to make a seed for gsl, */
	/* and record it in the `rand_history' */

	const int fd_0=open("/dev/urandom",O_RDONLY);
	fiopen("rand_history","w",rand_hist);
	read(fd_0,(void*)&seed,sizeof(seed));
	close(fd_0);
	gsl_rng_set(R,seed);
	fprintf(rand_hist,"%#lx\n",seed);
	fclose(rand_hist);

	/* Open files and start by recording their sizes */

	fd[0]=open("census",O_RDONLY);
	fd[1]=open("mut",O_RDONLY);
	fd[2]=open("recom",O_RDONLY);
	fd[3]=open("mig",O_RDONLY);


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
	unsigned int vec[nislands][nhap];
	double fitness[nhap];
	double cc[nislands];

	firead(fitness,sizeof(fitness),1,censusstream);
	firead(initial,sizeof(initial),1,censusstream);
	firead(cc,sizeof(cc),1,censusstream);
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
		/* If arg `mperiod=...' not set, always migrate if more than one island */
		if(mperiod==0) mperiod=1; 
	}
	else
		/* Otherwise never migrate since there is no place to migrate to */
		mperiod=ngen + 1;

	/* Now everything's loaded.  Begin simulation. */
	for(i=0;i<nsim;++i){
		for(j=0;j<nislands;++j)
			for(k=0;k<nhap;++k)
				vec[j][k]=initial[j][k];
		printf("Simulation %d:\n",i+1);
		for(j=0;j<ngen;++j)
			iterateCC(nislands,nhap,vec,(double*)mut,(double*)recom,	\
					(double*)mig,(double*)fitness,ifrecom!=0,	\
					!((j+1) % mperiod),cc);

		for(j=0;j<nislands;++j){
			for(k=0;k<nhap;++k)
				printf("%u ",vec[j][k]);
			printf("\n");
		}
		fflush(stdout);
	}
	return 0;
}
