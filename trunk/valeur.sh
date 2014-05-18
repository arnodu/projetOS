#!/bin/bash

if [ "$#" -eq 0 ]; then
    iter=10
else 	
	iter=$1
fi

if [ ! -d data ]; then  
	mkdir data
else
	rm data/*
fi

echo "temps d'execution de $iter itérations de 31-switch-many à nombre de yield constants et à nombre de threads variable"

for i in `seq 0 20`
do		
	j=$(($i*100))		
	for k in `seq 1 $iter`
	do 
		v=$(($v + $(bin/./31-switch-many $j 10 | cut -d " " -f 6)))
		w=$(($w + $(bin/./31-switch-many-pthread $j 10 | cut -d " " -f 6)))
	done
	vv=$(($v / $iter))	
	ww=$(($w / $iter))	
	echo "$j $vv" >> data/time_thread
	echo "$j $ww" >> data/time_thread_pthread
	
done

echo "temps d'execution de $iter itérations de 31-switch-many à nombre de threads constants et à nombre de yield variable"


for i in `seq 0 20`
do
	j=$(($i*100))
	for k in `seq 1 $iter`
	do 
		v=$(($v + $(bin/./31-switch-many 10 $j | cut -d " " -f 6)))
		w=$(($w + $(bin/./31-switch-many-pthread 10 $j | cut -d " " -f 6)))
	done	
	vv=$(($v / $iter))	
	ww=$(($w / $iter))	
	echo "$j $vv" >> data/time_yield
	echo "$j $ww" >> data/time_yield_pthread
done

echo "temps d'execution de $iter itérations de fibo"

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
	cc=$(echo "$c / $iter" | bc -l)
	dd=$(echo "$d / $iter" | bc -l)
	echo "$i $cc" >> data/time_fibo
	echo "$i $dd" >> data/time_fibo_pthread
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
set ylabel "Temps d'éxecution(s)" textcolor lt 1
set xlabel "fibo(x)" textcolor lt 1
set title "Temps d'execution de fibo en fonction de l'entrée" textcolor lt 1
plot "data/time_fibo" with lines, "data/time_fibo_pthread" with lines

set output "img/yield.png"
set ylabel "Temps d'éxecution(µs)" textcolor lt 1
set xlabel "nombre de yield fait" textcolor lt 1
set title "Temps d'execution de 31-switch-many à nombre de yield variables" textcolor lt 1
plot "data/time_yield" with lines, "data/time_yield_pthread" with lines

set output "img/thread.png"
set ylabel "Temps d'éxecution(µs)" textcolor lt 1
set xlabel "nombre de thread créés" textcolor lt 1
set title "Temps d'execution de 31-switch-many à nombre de thread variables" textcolor lt 1
plot "data/time_thread" with lines, "data/time_thread_pthread" with lines

__EOF
