main.out: main.o Convolution.o NonLinearActivation.o SubSampling.o VectorProbabilities.o
	g++ -o main.out main.o Convolution.o NonLinearActivation.o SubSampling.o VectorProbabilities.o /usr/lib/x86_64-linux-gnu/libopenblas.a -Wl,--start-group ${MKLROOT}/lib/intel64/libmkl_intel_lp64.a ${MKLROOT}/lib/intel64/libmkl_intel_thread.a ${MKLROOT}/lib/intel64/libmkl_core.a -Wl,--end-group -liomp5 -lpthread -lm -ldl
	rm main.o Convolution.o NonLinearActivation.o SubSampling.o VectorProbabilities.o

main.o: main.cpp Convolution.hpp NonLinearActivation.hpp SubSampling.hpp VectorProbabilities.hpp
	g++ -c main.cpp
Convolution.o: Convolution.cpp Convolution.hpp
	g++ -c Convolution.cpp -m64 -I${MKLROOT}/include
NonLinearActivation.o: NonLinearActivation.cpp NonLinearActivation.hpp
	g++ -c NonLinearActivation.cpp
SubSampling.o: SubSampling.cpp SubSampling.hpp
	g++ -c SubSampling.cpp
VectorProbabilities.o: VectorProbabilities.cpp VectorProbabilities.hpp
	g++ -c VectorProbabilities.cpp
