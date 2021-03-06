reset
set ylabel 'time(us)'
set style fill solid
set title 'perfomance comparison'
set term png enhanced font 'Verdana,10'
set output 'local/runtime.png'

plot [:][:260000]'local/output.txt' using 2:xtic(1) with histogram title 'naive transpose', \
'' using 3:xtic(1) with histogram title 'sse transpose'  , \
'' using 4:xtic(1) with histogram title 'sse prefetch transpose' , \
'' using 5:xtic(1) with histogram title 'avx transpose' , \
'' using 6:xtic(1) with histogram title 'avx prefetch transpose'
