if [ ! -d img ]; then
	mkdir img
fi

/usr/bin/gnuplot <<__EOF

set terminal png
set key on inside left top
set output "img/fibo.png"
plot "data/time_fibo_ours" with point, "data/time_fibo_pthread" with point

set output "img/yield.png"
plot "data/time_yield_ours" with point, "data/time_yield_pthread" with point

set output "img/thread.png"
plot "data/time_thread_ours" with point, "data/time_thread_pthread" with point

__EOF