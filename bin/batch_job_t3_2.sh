#!/bin/bash
# bash % 10000 1000 1e-7
[ $# -lt 1 ] && exit 0
#Run 1
cat << eof | ./settle create >/dev/null
16
2
eof
cat << eof | ./settle populate >/dev/null
1
1000000 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
2
1000000 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
eof
cat << eof | ./settle fitness >/dev/null
1.0 \
1.1 \
1.1 \
1.2 \
1.1 \
0.1 \
0.1 \
0.1 \
1.1 \
0.1 \
0.1 \
0.1 \
1.2 \
0.1 \
0.1 \
1.3
eof
mut_rate=`[ $# -gt 2 ] && echo $3 || echo 1e-7`
./makeMut morgan 4 $mut_rate
echo > ./makeMig symmetric 2 0.01
for r in 0 1e-{7..1} 1
do
	./makeRecom morgan 4 $r
	msg="Mutation: $mut_rate Recom: $r"
	echo $msg | tee
	echo $msg >&2
	./execWF $1 $2 mperiod=100 --fixrate | tee
done
