source /opt/intel/mkl/bin/mklvars.sh intel64
make -s -f Makefile

for x in gen_conv gen_mm_flip gen_mm_flip_intelmkl gen_mm_flip_openblas gen_mm_flip_pthread
do
    rm "$x"*.txt

    for ((i = 1000; i <= 15000; i+=1000))
    do
        echo "$x" "$i"

        for ((j = 1; j <= 5; j++))
        do
            /usr/bin/time -f %e -o ./"$x$j".txt -a ./main.out "$x" 1 1 filter.txt 3 matrix.txt "$i"
        done
    done

    paste "$x"1.txt "$x"2.txt "$x"3.txt "$x"4.txt "$x"5.txt >> "$x".txt
    rm "$x"1.txt "$x"2.txt "$x"3.txt "$x"4.txt "$x"5.txt

    python convert.py "$x".txt
    gnuplot -e "set xrange[0:16000]; set yrange[0:30]; set terminal png size 1600,900; set output '"$x".png'; plot '"$x".txt' using 1:2:3 with yerrorbars"
done
