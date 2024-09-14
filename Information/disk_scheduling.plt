set terminal png
set output 'Information/Disk_scheduling.png'
set title 'FCFS Disk Scheduling'
set xlabel 'Request Sequence'
set ylabel 'Cylinder'
set yrange [0:199]
set grid ytics
set ytic 50
set label sprintf('Total Movement: 346', '346') at graph 0.02, graph 0.98 font ',10' tc rgb 'black'
plot 'Information/disk_scheduling.dat' with linespoints title 'Total Movement' lt rgb 'green' pt 7 ps 1
