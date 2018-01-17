set format y "%3.1e"

set xlabel "Frame Count" font ", 15"
set ylabel "Energy" font ", 15"
set lmargin 13
set bmargin 4

plot 'Vars.dat' u 1 w l ls 6 title 'x-Energy', 'Vars.dat' u 2 w l ls 7 title 'y-Energy'

set term png
set output "Energy Budget.png"
replot
set term win

close