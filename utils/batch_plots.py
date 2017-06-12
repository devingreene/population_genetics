#!/usr/bin/python3
from tables_plotted import *
from sys import argv

parameters = None

if not 'noparams' in argv:
	if not 'cache' in argv:
		parameters = argv[-4:]
		argv = argv[:-4]
		cache = open('plots/.cache.'+argv[1],'w')
		cache.writelines([' '.join(parameters)])
	else:
		argv.remove('cache')
		cache = open('plots/.cache.'+argv[1],'r')
		parameters = cache.readline().split()
else:
	argv.remove('noparams')

if 'small' in argv:
	small=True
	argv.remove('small')
else: small=False

trunc = None
filetype = argv[-1]
try:
	int(filetype[0])
	filetype = 'png'
except:
	trunc = -1
	
for i in argv[1:trunc]:
	process_table("tables/table"+str(i),label="t"+str(i),\
		small=small,filetype=filetype,parameters=\
		parameters)
