set terminal png size 700,500 enhanced font "Helvetica,20"
set output 'output.png'
set size 1,1
set origin 0,0

set title 
set multiplot
set autoscale

set xlabel "Number of threads"
set ylabel "Time spent"

# Marges communes
set lmargin 10
set rmargin 2

# Axe des ordonnées commun
set yrange [0:15000]


#Tracés
plot "time_yield_pthread" using 1:2 title 'time\_thread\_pthread' lt rgb "red",\
	"time_yield_userspace" using 1:2 title 'time\_thread\_userspace' lt rgb "green"

unset multiplot