import sys

with open(sys.argv[1], 'r') as f:
    m = [[float(x) for x in a] for a in [line.split('\t') for line in f]];

with open(sys.argv[1], 'w') as f:
    i = 1;
    for a in m:
        mean = sum(a)/len(a);
        stderr = pow(sum([pow(x - mean, 2) for x in a])/(len(a) - 1), 0.5);
        f.write(str(1000*i) + " " + str(mean) + " " + str(stderr) + "\n");
        i += 1;
