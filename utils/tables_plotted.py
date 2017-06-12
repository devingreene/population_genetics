""" Plotted Nina's tables in article """

import matplotlib.pyplot as plt,numpy as np,re
from time import asctime

def process_table(file,**kwargs):
	
	h = open(file,"r")
	lines = h.readlines()

	lr = []
	fr = []
	for i in range(0,len(lines),2):
		target_indices = slice(*re.search('-?\d\D*$',lines[i]).span())
		target = lines[i][target_indices]
		lr.append(int(target))
		fr.append(float(lines[i+1].split()[-1]))
	# Change last to 0 (1e0) rather than 1
	lr[-1] = 0
	plotit(lr,fr,**kwargs)

def plotit(lr,fr,abscissa=1,label=None,ndashes=5,small=False,filetype="png",parameters=None):
	
	""" General function for plotting tables; 
		Addes gray bar to separate 0 from log 
		scaled values. """

	if(len(lr) != len(fr)):
		raise Exception("Vector mismatch")
	
	if(label==None):
		raise Exception("Please choose label for file name")

	# Offset slightly since out of log range
	lr[0] = lr[1]-1.5

	#convert to arrays
	lr = np.array(lr)
	fr = np.array(fr)

	# Setting up figure
	fig = plt.figure(figsize=(3.5 if small else 12,2 if small else 6))
	plt.plot(lr[1:],fr[1:],lr[0],fr[0],marker='v',markersize=6 if small else 10,color='red',zorder=1,figure=fig)
	ax = plt.gca()
	# Put all axes children under, so strips overlay
	for i in ax.get_children():
		i.zorder=0

	ax.set_xticks(lr)
	# Label x tickmarks
	ax.set_xticklabels([0]+["{:0.0e}".format(abscissa*10**x) for x in lr[1:-1]]+[abscissa],\
		size=6 if small else None)
	if small:
		# I need to scale the axis lables
		ax.get_yaxis().set_tick_params(labelsize=6,length=2)
		ax.get_xaxis().set_tick_params(length=2)

	# Place white bars half-way between 0 and lowest positive tick mark
	spanc = np.array(lr[0]+lr[1])/2
	spanw = plt.xlim()
	spanc += (spanw[1]-spanw[0])/20*np.array([-1,1])
	spancc = np.linspace(*spanc,2*ndashes+2)

	for i in range(0,2*ndashes+2,2):
		plt.axvspan(*spancc[i:i+2],-0.01,1.01,clip_on=False,color=ax.get_axis_bgcolor(),zorder=2)
	
	# Adjust x,y ranges
	plt.xlim(lr[0]-0.5,lr[-1]+0.5)
	plt.ylim(-0.02,1.02)
	
	# Legend
	
	if parameters:
		text = asctime()+"\n\
loci = {}\n\
mu = {}\n\
N = {}\n\
Is = {}\n".format(*parameters)

		ax.text(lr[0],0.5,text)

	plt.savefig('plots/plot_'+label+'.'+filetype)
