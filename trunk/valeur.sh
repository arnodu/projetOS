

# temps d'execution à nombre de yield constants et à nombre de threads variable

for i in `seq 10 1000`
do
	v=$(bin/./31-switch-many 10 $i | cut -d " " -f 6)
	echo "$i $v" >> time_yield_$1
done

# temps d'execution à nombre de threads constants et à nombre de yield variable


for i in `seq 10 1000`
do
	v=$(bin/./31-switch-many $i 10 | cut -d " " -f 6)
	echo "$i $v" >> time_thread_$1
done

# temps d'execution de fibo

for i in `seq 1 16`
do
	a=$(date +%s.%N)	
	bin/./51-fibonacci  $i
	b=$(date +%s.%N)		
	c=$(echo "$b - $a" | bc)
	echo "$i $c" >> time_fibo_$1
done