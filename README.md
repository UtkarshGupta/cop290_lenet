# Convolution.cpp

## Arguments
### Argument 1: convolution/matrixmult & withpadding/withoutpadding
                Seperate the two choices with _
### Argument 2: Padding Size
                Padding Size is necessary, even if you are not using it
### Argument 3: Kernel File
                Must be square a matrix
### Argument 4: Kernel size
                Number of rows/columns
### Argument 5: Input File
                Must be square matrix
### Argument 6: Input size
                Number of rows/columns

## Examples
                ./Convolution.out convolution_withpadding 1 ./kernel.txt 2 ./matrix.txt 4
                ./Convolution.out matrixmult_withoutpadding 0 ./kernel.txt 3 ./matrix.txt 5

# NonLinearActivations.cpp

## Arguments
### Argument 1: relu/tanh
### Argument 2: Input file
### Argument 3: Number of rows in input
### Argument 4: Number of columns in Input

## Examples
                ./NonLinearActivations.out relu ./kernel.txt 2 2
                ./NonLinearActivations.out tanh ./matrix.txt 4 4

# SubSampling.cpp

## Arguments
### Argument 1: maxpooling/avgpooling
### Argument 2: Input File
                Must be square matrix
### Argument 3: Input size
                Number of rows/columns
### Argument 4: Filter size
                Number of rows/columns
### Argument 5: Stride
                Number of steps at a time

## Examples
                ./SubSampling.out maxpooling ./matrix.txt 4 2 2
                ./SubSampling.out avgpooling ./matrix.txt 4 3 1

# VectorProbabilities.cpp

## Arguments
### Argument 1: sigmoid/softmax
### Argument 2: Input File
                Must be square matrix
### Argument 3: Input size
                Number of rows/columns

## Examples
                ./VectorProbabilities.out sigmoid ./kernel.txt 4
                ./VectorProbabilities.out softmax ./kernel.txt 4
