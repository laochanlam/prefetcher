reset
set ylabel 'time(us)'
set style fill solid
set title 'perfomance comparison'
set term png enhanced font 'Verdana,10'
set output 'runtime.png'

plot [:][:260000]'output.txt' using 2:xtic(1) with histogram title 'naive transpose', \
'' using 3:xtic(1) with histogram title 'sse transpose'  , \
'' using 4:xtic(1) with histogram title 'sse prefetch transpose'
