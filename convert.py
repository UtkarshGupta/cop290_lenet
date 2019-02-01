import sys
from math import sqrt

with open(sys.argv[1], 'r+') as f:
    m = [line.split('\t') for line in f];
    m = [[float(x) for x in a] for a in m];
    f.seek(0);

    i = 1;
    for a in m:
        mean = sum(a)/5;
        stdev = 0;
        for x in a:
            stdev += (x - mean)**2;
        stdev = sqrt(stdev/5);
        n = 1000*i;
        i += 1;
        f.write(str(n) + " " + str(mean) + " " + str(stdev) + "\n");
