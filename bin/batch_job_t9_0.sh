#!/bin/bash
# bash % 10000 1000 1e-8 > table9
[ $# -lt 1 ] && exit 0
#Run 1
cat << eof | ./settle create >/dev/null
4
1
eof
cat << eof | ./settle populate >/dev/null
1
10000000 0 0 0
eof
cat << eof | ./settle fitness >/dev/null
1 0.1 0.1 3
eof
mut_rate=`[ $# -gt 2 ] && echo $3 || echo 1e-7`
./makeMut morgan 2 $mut_rate
for r in 0 1e-{7..1} 1
do
	./makeRecom morgan 2 $r
	msg="Mutation: $mut_rate Recom: $r"
	echo $msg | tee >&2
	echo $msg | tee
	./execWF $1 $2 mperiod=1 --fixrate | tee
done
