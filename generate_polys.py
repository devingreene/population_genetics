from sympy.parsing.sympy_parser import parse_expr

__all__ = ['append_and_add_symbols','generate_polys','symbols']

symbols = set()

def append_and_add_symbols(lst,str_):
    expr = parse_expr(str_)
    lst.append(expr)
    symbols.update(expr.free_symbols)

def _switch(bit):
    return '0' if bit=='1' else '1' if bit == '0' else None

def generate_polys():
    poly_list = []

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
                # add polynomial
                append_and_add_symbols(poly_list,'(w{} - w{})*(w{} - w{})'.format(a0,a1,b0,b1))
    
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
                        append_and_add_symbols(poly_list,
                                ('(w{} - w{})*'.format(a[:k]+ch[0]+a[k:l]+ch[1]+a[l:],
                                    a[:k]+_switch(ch[0])+a[k:l]+_switch(ch[1])+a[l:]) +
                                    '(w{} - w{})'.format(b[:k]+ch[0]+b[k:l]+ch[1]+b[l:],
                                        b[:k]+_switch(ch[0])+b[k:l]+_switch(ch[1])+b[l:]))
                                    )
    
    # See comments in two-mutant block to understand how this works.
    for k in range(3):
        for l in range(k,3):
            for m in range(l,3):
                for i in range(4):
                    for j in range(i+1,4):
                        a,b = ('{:02b}'.format(x) for x in (i,j))
                        for ch in (('0','0','0'),('0','0','1'),('0','1','0'),
                                ('0','1','1')):
                            append_and_add_symbols(poly_list,
                                    ('(w{} - w{})*'.format(
                                        a[:k]+ch[0]+a[k:l]+ch[1]+a[l:m]+ch[2]+a[m:],
                                        a[:k]+_switch(ch[0])+a[k:l]+_switch(ch[1])+a[l:m]+_switch(ch[2])+a[m:]) + 
                                        '(w{} - w{})'.format(
                                            b[:k]+ch[0]+b[k:l]+ch[1]+b[l:m]+ch[2]+b[m:],
                                            b[:k]+_switch(ch[0])+b[k:l]+_switch(ch[1])+b[l:m]+_switch(ch[2])+b[m:]))
                                        )
                                    
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
                                append_and_add_symbols(poly_list,
                                        ('(w{} - w{})*'.format(
                                            a[:k]+ch[0]+a[k:l]+ch[1]+a[l:m]+ch[2]+a[m:n]+ch[3]+a[n:],
                                            a[:k]+_switch(ch[0])+
                                            a[k:l]+_switch(ch[1])+
                                            a[l:m]+_switch(ch[2])+
                                            a[m:n]+_switch(ch[3])+a[n:])+
                                            '(w{} - w{})'.format(
                                                b[:k]+ch[0]+b[k:l]+ch[1]+b[l:m]+ch[2]+b[m:n]+ch[3]+b[n:],
                                                b[:k]+_switch(ch[0])+
                                                b[k:l]+_switch(ch[1])+
                                                b[l:m]+_switch(ch[2])+
                                                b[m:n]+_switch(ch[3])+b[n:]))
                                            )
    return poly_list
