
if [ $# -eq 0 ]
 then
	echo "veuillez rentrez un parametres: ours pour nos threads pthread pour les threads posix"

else
	if [ ! -d data ]; then  
		mkdir data
	fi

# temps d'execution à nombre de yield constants et à nombre de threads variable

	for i in `seq 10 1000`
	do
		v=$(bin/./31-switch-many $i 10 | cut -d " " -f 6)
		echo "$i $v" >> data/time_thread_$1
	done

# temps d'execution à nombre de threads constants et à nombre de yield variable


	for i in `seq 10 1000`
	do
		v=$(bin/./31-switch-many 10 $i | cut -d " " -f 6)
		echo "$i $v" >> data/time_yield_$1
	done

# temps d'execution de fibo

	for i in `seq 1 16`
	do
		a=$(date +%s.%N)	
		bin/./51-fibonacci  $i
		b=$(date +%s.%N)		
		c=$(echo "$b - $a" | bc)
		echo "$i $c" >> data/time_fibo_$1
	done

fi