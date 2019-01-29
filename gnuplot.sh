source /opt/intel/mkl/bin/mklvars.sh intel64
make -s -f Makefile

for x in convolution matrixmult_flip matrixmult_flip_intelmkl matrixmult_flip_openblas matrixmult_flip_pthread matrixmult_flip_quick
do
    rm n.txt "$x"*.txt

    for ((i = 10; i <= 210; i+=10))
    do
        echo "$i" >> n.txt

        rm matrix.txt
        for ((j = 1; j <= $i; j++))
        do
            for ((k = 1; k <= $i; k++))
            do
                echo "$j" >> ./matrix.txt
            done
        done

        for ((j = 1; j <= 5; j++))
        do
            /usr/bin/time -f %e -o ./"$x$j".txt -a ./main.out "$x" 1 ./kernel.txt 3 ./matrix.txt "$i" >/dev/null
        done
    done

    paste n.txt "$x"1.txt "$x"2.txt "$x"3.txt "$x"4.txt "$x"5.txt >> "$x".txt
    rm "$x"1.txt "$x"2.txt "$x"3.txt "$x"4.txt "$x"5.txt
done

# Convert from x y1 y2 y3 y4 y5 to x ymean ystddev

#gnuplot> plot "./convolution.txt" using 1:2:3 with yerrorbars
#gnuplot> plot "./matrixmult_flip.txt" using 1:2:3 with yerrorbars
#gnuplot> plot "./matrixmult_flip_intelmkl.txt" using 1:2:3 with yerrorbars
#gnuplot> plot "./matrixmult_flip_openblas.txt" using 1:2:3 with yerrorbars
#gnuplot> plot "./matrixmult_flip_pthread.txt" using 1:2:3 with yerrorbars
#gnuplot> plot "./matrixmult_flip_quick.txt" using 1:2:3 with yerrorbars
