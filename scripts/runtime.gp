reset
set ylabel 'time(us)'
set style fill solid
set title 'perfomance comparison'
set term png enhanced font 'Verdana,10'
set output 'runtime.png'

plot [:][:170000]'output.txt' using 2:xtic(1) with histogram title 'PFDIST = 2', \
'' using 3:xtic(1) with histogram title 'PFDIST =  4'  , \
'' using 4:xtic(1) with histogram title 'PFDIST =  6'  , \
'' using 5:xtic(1) with histogram title 'PFDIST =  8'  , \
'' using 6:xtic(1) with histogram title 'PFDIST = 10' , \
'' using 7:xtic(1) with histogram title 'PFDIST = 12' , \
'' using 8:xtic(1) with histogram title 'PFDIST = 14'
