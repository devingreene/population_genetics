#define Error(string) {fprintf(stderr,"%s\n",string);	\
    exit(1);}


#define fiopen(arg1,arg2,arg3) {					\
    if((arg3=fopen(arg1,arg2))==NULL)					\
      fprintf(stderr,"%s\n",					\
	      "Error opening file");				\
  }

#define firead(arg1,arg2,arg3,arg4) {fread(arg1,arg2,arg3,arg4);\
  if(feof(arg4) || ferror(arg4)){				\
    fprintf(stderr,"%s\n","Error reading file");		\
  }								\
}

#define fiwrite(arg1,arg2,arg3,arg4) {fwrite(arg1,arg2,arg3,arg4);	\
  if(ferror(arg4))							\
    fprintf(stderr,"%s\n","Error writing to file");			\
  }

#define fiseek(arg1,arg2,arg3) {fseek(arg1,arg2,arg3);\
	if(ferror(arg1))	\
		fprintf(stderr,"%s\n","Error seeking in file");\
}
