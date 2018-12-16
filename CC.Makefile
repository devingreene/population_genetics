.PHONY : all

all : execCC makeMut makeRecom makeMig settle readMut readMig readRecom readParameters

ifeq ($(CFLAGS),-ggdb)
OPTIMIZATION_FLAG=
else 
OPTIMIZATION_FLAG = -O2 -Wno-unused-result
endif

MAIN_SOURCE_FILES = migstep.c darwinstep.c mutstep.c recomstep.c iterateCC.c mainCC.c 

AUXILIARY_SOURCE_FILES = makeMut.c makeRecom.c makeMig.c settle.c readMut.c readMig.c readRecom.c readParameters.c

AUXILIARY_BINARIES = makeMut makeRecom makeMig settle readMut readMig readRecom readParameters

execCC : $(MAIN_SOURCE_FILES) 
	cc -Wall -Wextra  $(CFLAGS) $(OPTIMIZATION_FLAG) $(MAIN_SOURCE_FILES) \
	-lgsl -lgslcblas -lm -o execCC

makeMut : makeMut.c
	cc -Wall -Wextra  $(CFLAGS) $(OPTIMIZATION_FLAG) makeMut.c -o makeMut

makeRecom : makeRecom.c
	cc -Wall -Wextra  $(CFLAGS) $(OPTIMIZATION_FLAG) makeRecom.c -o makeRecom

makeMig : makeMig.c
	cc -Wall -Wextra  $(CFLAGS) $(OPTIMIZATION_FLAG) makeMig.c -o makeMig

settle : settle.c
	cc -Wall -Wextra  $(CFLAGS) $(OPTIMIZATION_FLAG) settle.c -o settle

readMut : readMut.c
	cc -Wall -Wextra $(CFLAGS) $(OPTIMIZATION_FLAG) readMut.c -o readMut -lm

readMig : readMig.c
	cc -Wall -Wextra $(CFLAGS) $(OPTIMIZATION_FLAG) readMig.c -o readMig -lm

readRecom : readRecom.c
	cc -Wall -Wextra $(CFLAGS) $(OPTIMIZATION_FLAG) readRecom.c -o readRecom -lm

readParameters : readParameters.c
	cc -Wall -Wextra $(CFLAGS) $(OPTIMIZATION_FLAG) readParameters.c -o readParameters -lm

clean:
	rm execCC $(AUXILIARY_BINARIES)

