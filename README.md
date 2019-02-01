# Convolution [conv: filter flipped][mm: filter not flipped]
## Speed: mm_flip < conv < mm_flip_pthread < mm_flip_openblas < mm_flip_intelmkl
## Arguments
### Argument 1: gen_ + conv/mm_ + flip_ + pthread/openblas/intelmkl [String]
                First & last 2 choices are optional. Last choice only works with matrix multiplication. First choice generates matrix with all elements equal to a float value between 0 & 100.
### Argument 2: Padding Size [Integer >= 0]
                Enter 0 if you don't require padding
### Argument 3: Stride [Integer > 0]
                Number of steps at a time
### Argument 4: Filter File [File Location]
                Must be a square matrix, in column-major format, with one value per line
### Argument 5: Filter size [Integer > 0]
                Number of rows/columns
### Argument 6: Input File [File Location]
                Must be a square matrix, in column-major format, with one value per line
### Argument 7: Input size [Integer > 0]
                Number of rows/columns

## Examples
                ./main.out conv 0 1 filter.txt 2 matrix.txt 4
                ./main.out mm_flip_intelmkl 1 1 filter.txt 3 matrix.txt 5

# SubSampling
## Arguments
### Argument 1: ss_ + max/avg [String]
### Argument 2: Input File [File Location]
                Must be a square matrix, in column-major format, with one value per line
### Argument 3: Input size [Integer > 0]
                Number of rows/columns
### Argument 4: Filter size [Integer > 0]
                Number of rows/columns
### Argument 5: Stride [Integer > 0]
                Number of steps at a time

## Examples
                ./main.out ss_max matrix.txt 5 2 1
                ./main.out ss_avg matrix.txt 5 3 2

# NonLinearActivation
## Arguments
### Argument 1: nla_ + relu/tanh [String]
### Argument 2: Input file [File Location]
                Must be in column-major format, with one value per line
### Argument 3: Number of rows in input [Integer > 0]
### Argument 4: Number of columns in Input [Integer > 0]

## Examples
                ./main.out nla_relu matrix.txt 5 5
                ./main.out nla_tanh matrix.txt 5 5

# VectorProbabilities
## Arguments
### Argument 1: vp_ + sigmoid/softmax [String]
### Argument 2: Input File [File Location]
                Must be a square matrix, in column-major format, with one value per line
### Argument 3: Input size [Integer > 0]
                Number of rows/columns

## Examples
                ./main.out vp_sigmoid matrix.txt 25
                ./main.out vp_softmax matrix.txt 25
