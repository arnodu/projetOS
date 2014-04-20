#!/bin/bash

if [ "$#" -eq 0 ]; then
    iter=50
else 	
	iter=$1
fi

if [ ! -d data ]; then  
	mkdir data
else
	rm data/*
fi

echo "temps d'execution de $iter itérations de 31-switch-many à nombre de yield constants et à nombre de threads variable"

for i in `seq 0 100`
do		
	j=$(($i*20))		
	for k in `seq 1 $iter`
	do 
		v=$(($v + $(bin/./31-switch-many $j 10 | cut -d " " -f 6)))
		w=$(($w + $(bin/./31-switch-many-pthread $j 10 | cut -d " " -f 6)))
	done	
	echo "$j $v" >> data/time_thread
	echo "$j $w" >> data/time_thread_pthread
	
done

echo "temps d'execution de $iter itérations de 31-switch-many à nombre de threads constants et à nombre de yield variable"


for i in `seq 0 100`
do
	j=$(($i*20))
	for k in `seq 1 $iter`
	do 
		v=$(($v + $(bin/./31-switch-many 10 $j | cut -d " " -f 6)))
		w=$(($w + $(bin/./31-switch-many-pthread 10 $j | cut -d " " -f 6)))
	done	
	echo "$j $v" >> data/time_yield
	echo "$j $w" >> data/time_yield_pthread
done

echo "temps d'execution de $iter itérations de 31-switch-many de fibo"

for i in `seq 1 20`
do
	a=$(date +%s.%N)	
	for j in `seq 1 $iter` 
	do
		val=$(bin/./51-fibonacci  $i)
	done
	b=$(date +%s.%N)		
	c=$(echo "$b - $a" | bc)
	a=$(date +%s.%N)	
	for k in `seq 1 $iter` 
	do
		val=$(bin/./51-fibonacci-pthread  $i)
	done
	b=$(date +%s.%N)		
	d=$(echo "$b - $a" | bc)
	echo "$i $c" >> data/time_fibo
	echo "$i $d" >> data/time_fibo_pthread
done

echo "tracé des courbes"

if [ ! -d img ]; then
	mkdir img
else
	rm img/*
fi

/usr/bin/gnuplot <<__EOF

set terminal png
set key on inside left top
set output "img/fibo.png"
plot "data/time_fibo" with point, "data/time_fibo_pthread" with point

set output "img/yield.png"
plot "data/time_yield" with point, "data/time_yield_pthread" with point

set output "img/thread.png"
plot "data/time_thread" with point, "data/time_thread_pthread" with point

__EOF