import sys
import time
import re

try:
    inp = input('''Specify the name of the file you wish to output the data to.  '''\
            '''If you want the data to be written to standard output, hit '''\
            '''<Return>.\n''')
except EOFError:
    inp = ''
    

if inp.strip(): 
    try:
        file = open(inp,'x')
    except FileExistsError:
        inp2 = input("ARE YOU ABSOLUTELY SURE THAT YOU WANT TO OVERWRITE"\
                " {}?: ".format(inp))
        if re.fullmatch("y|yes|ja",inp2.strip(),flags=re.I): 
            file = open(inp,'w')
        else:
            print("Exiting program",file=sys.stderr)
            sys.exit(1)

else: file = sys.stdout

def switch(bit):
    return '0' if bit=='1' else '1' if bit == '0' else None

# Timestamp
print("Data computed: {}".format(time.asctime()),file=file)
print("-"*15,"One mutants","-"*15, file=file)
for k in range(5):
    # i,j are the two backgrounds
    for i in range(16):
        for j in range(i+1,16):
            # Fill with zeros on the left and convert to base 2
            a = '{:04b}'.format(i)
            b = '{:04b}'.format(j)
            # Insert flipped loci appropriately
            a0 = a[:k] + '0' + a[k:]
            a1 = a[:k] + '1' + a[k:]
            b0 = b[:k] + '0' + b[k:]
            b1 = b[:k] + '1' + b[k:]
            # Print polynomial
            print('(w{} - w{})*(w{} - w{})'.format(a0,a1,b0,b1), file=file)

print("-"*15,"Two mutants","-"*15, file=file)
#k,l represent where among changing loci background loci are placed.
# There are n+1 positions to choose from among n background loci
for k in range(4):
    for l in range(k,4):
        #i,j are backgrounds -> 3 loci means 8 backgrounds, taken in pairs
        for i in range(8):
            for j in range(i+1,8):
                # See above in one mutant case
                a,b = ('{:03b}'.format(x) for x in (i,j))
                # Two initial states
                for ch in (('0','0'),('0','1')):
                    # Insert loci which change appropriately
                    print('(w{} - w{})*'.format(a[:k]+ch[0]+a[k:l]+ch[1]+a[l:],
                        a[:k]+switch(ch[0])+a[k:l]+switch(ch[1])+a[l:]),end='', file=file)
                    print('(w{} - w{})'.format(b[:k]+ch[0]+b[k:l]+ch[1]+b[l:],
                        b[:k]+switch(ch[0])+b[k:l]+switch(ch[1])+b[l:]), file=file)

print("-"*15,"Three mutants","-"*15, file=file)
# See comments in two-mutant block to understand how this works.
for k in range(3):
    for l in range(k,3):
        for m in range(l,3):
            for i in range(4):
                for j in range(i+1,4):
                    a,b = ('{:02b}'.format(x) for x in (i,j))
                    for ch in (('0','0','0'),('0','0','1'),('0','1','0'),
                            ('0','1','1')):
                        print('(w{} - w{})*'.format(
                            a[:k]+ch[0]+a[k:l]+ch[1]+a[l:m]+ch[2]+a[m:],
                            a[:k]+switch(ch[0])+a[k:l]+switch(ch[1])+a[l:m]+switch(ch[2])+a[m:]),
                            end='', file=file)
                        print('(w{} - w{})'.format(
                            b[:k]+ch[0]+b[k:l]+ch[1]+b[l:m]+ch[2]+b[m:],
                            b[:k]+switch(ch[0])+b[k:l]+switch(ch[1])+b[l:m]+switch(ch[2])+b[m:]), file=file)
                                
print("-"*15,"Four mutants","-"*15, file=file)
# See comments in two-mutant block to understand how this works.
for k in range(2):
    for l in range(k,2):
        for m in range(l,2):
            for n in range(m,2):
                for i in range(2):
                    for j in range(i+1,2):
                        a,b = ('{:01b}'.format(x) for x in (i,j))
                        # iterate thru (0,0,0,0),(0,0,0,1),(0,0,1,0) ...
                        for ch in ( list('0{:03b}'.format(i)) for i in range(8)):
                            print('(w{} - w{})*'.format(
                                a[:k]+ch[0]+a[k:l]+ch[1]+a[l:m]+ch[2]+a[m:n]+ch[3]+a[n:],
                                a[:k]+switch(ch[0])+
                                a[k:l]+switch(ch[1])+
                                a[l:m]+switch(ch[2])+
                                a[m:n]+switch(ch[3])+a[n:]), end='', file=file)
                            print('(w{} - w{})'.format(
                                b[:k]+ch[0]+b[k:l]+ch[1]+b[l:m]+ch[2]+b[m:n]+ch[3]+b[n:],
                                b[:k]+switch(ch[0])+
                                b[k:l]+switch(ch[1])+
                                b[l:m]+switch(ch[2])+
                                b[m:n]+switch(ch[3])+b[n:]), file=file)
