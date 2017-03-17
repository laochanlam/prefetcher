reset
set ylabel 'time(us)'
set style fill solid
set title 'perfomance comparison'
set term png enhanced font 'Verdana,10'
set output 'runtime.png'

plot [:][:170000]'output.txt' using 2:xtic(1) with histogram title 'T0', \
'' using 3:xtic(1) with histogram title 'T1'  , \
'' using 4:xtic(1) with histogram title 'T2'  , \
'' using 5:xtic(1) with histogram title 'NTA' 
