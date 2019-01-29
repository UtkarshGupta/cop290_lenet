# Convolution [stride = 1][convolution = matrixmult_flip]
## Speed: matrixmult < pthread < convolution < openblas < intelmkl < quick
## Arguments
### Argument 1: convolution/matrixmult_ + flip_ + pthread/openblas/intelmkl/quick [String]
                Last 2 choices are optional, last choice only works with matrixmult
### Argument 2: Padding Size [Integer >= 0]
                Enter 0 if you don't require padding
### Argument 3: Kernel File [File Location]
                Must be a square matrix, in column-major format, with one value per line
### Argument 4: Kernel size [Integer > 0]
                Number of rows/columns
### Argument 5: Input File [File Location]
                Must be a square matrix, in column-major format, with one value per line
### Argument 6: Input size [Integer > 0]
                Number of rows/columns

## Examples
                ./main.out convolution 0 ./kernel.txt 2 ./matrix.txt 4
                ./main.out matrixmult_flip_quick 1 ./kernel.txt 3 ./matrix.txt 5

# NonLinearActivation
## Arguments
### Argument 1: nonlinearactivation_ + relu/tanh [String]
### Argument 2: Input file [File Location]
                Must be in column-major format, with one value per line
### Argument 3: Number of rows in input [Integer > 0]
### Argument 4: Number of columns in Input [Integer > 0]

## Examples
                ./main.out nonlinearactivation_relu ./kernel.txt 3 3
                ./main.out nonlinearactivation_tanh ./matrix.txt 5 5

# SubSampling
## Arguments
### Argument 1: subsampling_ + maxpooling/avgpooling [String]
### Argument 2: Input File [File Location]
                Must be a square matrix, in column-major format, with one value per line
### Argument 3: Input size [Integer > 0]
                Number of rows/columns
### Argument 4: Filter size [Integer > 0]
                Number of rows/columns
### Argument 5: Stride [Integer > 0]
                Number of steps at a time

## Examples
                ./main.out subsampling_maxpooling ./matrix.txt 5 2 2
                ./main.out subsampling_avgpooling ./matrix.txt 5 3 1

# VectorProbabilities
## Arguments
### Argument 1: vectorprobabilities_ + sigmoid/softmax [String]
### Argument 2: Input File [File Location]
                Must be a square matrix, in column-major format, with one value per line
### Argument 3: Input size [Integer > 0]
                Number of rows/columns

## Examples
                ./main.out vectorprobabilities_sigmoid ./kernel.txt 9
                ./main.out vectorprobabilities_softmax ./kernel.txt 9
