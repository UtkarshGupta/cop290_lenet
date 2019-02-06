CC = g++
CFLAGS = -Wall -Ofast -flto -fuse-linker-plugin -funroll-loops
OBJECTS = main.o lenet.o Convolution.o SubSampling.o NonLinearActivation.o VectorProbabilities.o
OPENBLAS = /usr/lib/x86_64-linux-gnu/libopenblas.a
INTELMKL = -Wl,--start-group \
			${MKLROOT}/lib/intel64/libmkl_core.a \
			${MKLROOT}/lib/intel64/libmkl_intel_lp64.a \
			${MKLROOT}/lib/intel64/libmkl_intel_thread.a \
			-Wl,--end-group \
			-liomp5 -lpthread -lm -ldl
OPENCV = `pkg-config --cflags --libs opencv`

main.out: $(OBJECTS)
	$(CC) $(CFLAGS) -o main.out $(OBJECTS) $(OPENBLAS) $(INTELMKL) $(OPENCV)
	$(RM) $(OBJECTS)

main.o:
	$(CC) $(CFLAGS) -c main.cpp
lenet.o:
	$(CC) $(CFLAGS) -c lenet.cpp
Convolution.o:
	$(CC) $(CFLAGS) -c Convolution.cpp -m64 -I${MKLROOT}/include
SubSampling.o:
	$(CC) $(CFLAGS) -c SubSampling.cpp
NonLinearActivation.o:
	$(CC) $(CFLAGS) -c NonLinearActivation.cpp
VectorProbabilities.o:
	$(CC) $(CFLAGS) -c VectorProbabilities.cpp
