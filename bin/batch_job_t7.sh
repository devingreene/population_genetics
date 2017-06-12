#!/bin/bash
# bash % 1000 10 1e-8
[ $# -lt 1 ] && exit 0
#Run 1
cat << eof | ./settle create >/dev/null
64
1
eof
cat << eof | ./settle populate >/dev/null
1
10000000 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
eof
cat << eof | ./settle fitness >/dev/null
1.0 \
1.1 \
1.1 \
1.2 \
1.1 \
1.2 \
1.2 \
1.3 \
1.1 \
0.1 \
0.1 \
0.1 \
0.1 \
0.1 \
0.1 \
0.1 \
1.1 \
0.1 \
0.1 \
0.1 \
0.1 \
0.1 \
0.1 \
0.1 \
1.2 \
0.1 \
0.1 \
0.1 \
0.1 \
0.1 \
0.1 \
0.1 \
1.1 \
0.1 \
0.1 \
0.1 \
0.1 \
0.1 \
0.1 \
0.1 \
1.2 \
0.1 \
0.1 \
0.1 \
0.1 \
0.1 \
0.1 \
0.1 \
1.2 \
0.1 \
0.1 \
0.1 \
0.1 \
0.1 \
0.1 \
0.1 \
1.3 \
0.1 \
0.1 \
0.1 \
0.1 \
0.1 \
0.1 \
3.0
eof
mut_rate=`[ $# -gt 2 ] && echo $3 || echo 1e-7`
./makeMut morgan 6 $mut_rate
for r in 0 1e-{7..1} 1
do
	./makeRecom morgan 6 $r
	msg="Mutation: $mut_rate Recom: $r"
	echo $msg | tee
	echo $msg >&2
	./execWF $1 $2 mperiod=1 --fixrate | tee
done
