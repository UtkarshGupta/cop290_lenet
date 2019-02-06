source /opt/intel/mkl/bin/mklvars.sh intel64
make -s -j8

for x in gen_mm gen_mm_intelmkl gen_mm_openblas gen_mm_pthread gen_conv gen_conv_pthread
do
    for ((i = 1000; i <= 15000; i+=1000))
    do
        for ((j = 1; j <= 5; j++))
        do
            /usr/bin/time -f %e -o "$x$j".txt -a ./main.out "$x" 1 matrix.txt "$i" filter.txt 3
        done
    done

    paste "$x"*.txt >> "$x".txt
    python sample.py "$x".txt
    gnuplot -e "set xrange[0:16000]; set yrange[0:10]; set term eps;
            set output '"$x".eps'; plot '"$x".txt' using 1:2:3 with yerrorbars"
    rm "$x"*.txt
done
