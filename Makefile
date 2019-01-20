main: Convolution.out NonLinearActivations.out VectorProbabilities.out SubSampling.out

Convolution.out: Convolution.cpp
	g++ Convolution.cpp -o Convolution.out
NonLinearActivations.out: NonLinearActivations.cpp
	g++ NonLinearActivations.cpp -o NonLinearActivations.out
VectorProbabilities.out: VectorProbabilities.cpp
	g++ VectorProbabilities.cpp -o VectorProbabilities.out
SubSampling.out: SubSampling.cpp
	g++ SubSampling.cpp -o SubSampling.out
