# Digit recognition
## Arguments
### Argument 1: errorrate/recognize_ + invert [String]
                errorrate: iterate over the MNIST test set and find the digit recognition error rate
                recognize: recognize the digit in the given image file
                invert [Optional]: inverts input image [recognize only]
### Argument 2: Image [File Location] [Optional]
                Must be a 28x28 image file with digit in black or use invert

## Examples
                ./main.out errorrate
                ./main.out recognize_invert image.png

# Convolution [Stride = 1]
## Speed: gen_mm < gen_mm_intelmkl ~ gen_mm_openblas < gen_mm_pthread << gen_conv << gen_conv_pthread
## Arguments
### Argument 1: gen_ + conv/mm_ + flip_ + intelmkl/openblas/pthread [String]
                gen [Optional]: generates matrix with all elements equal to a random float
                conv: filter flipped / mm: filter not flipped
                flip [Optional]: flip filter
                pthread [Optional]: run conv/mm parallely
                intelmkl/openblas [Optional]: use intelmkl/openblas blas operations [mm only]
### Argument 2: Padding Size [Integer >= 0]
                Enter 0 if you don't require padding
### Argument 3: Input [File Location]
                Must be a square matrix, in column-major format, with one value per line
### Argument 4: Input size [Integer > 0]
                Number of rows/columns
### Argument 5: Filter [File Location]
                Must be a square matrix, in column-major format, with one value per line
### Argument 6: Filter size [Integer > 0]
                Number of rows/columns

## Examples
                ./main.out conv_pthread 0 matrix.txt 5 filter.txt 3
                ./main.out mm_flip_pthread 0 matrix.txt 5 filter.txt 3

# SubSampling
## Arguments
### Argument 1: ss_ + max/avg [String]
### Argument 2: Input [File Location]
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
### Argument 2: Input [File Location]
                Must be a matrix, in column-major format, with one value per line
### Argument 3: Number of rows [Integer > 0]
### Argument 4: Number of columns [Integer > 0]

## Examples
                ./main.out nla_relu matrix.txt 5 5
                ./main.out nla_tanh matrix.txt 5 5

# VectorProbabilities
## Arguments
### Argument 1: vp_ + softmax/sigmoid [String]
### Argument 2: Input [File Location]
                Must be an array, with one value per line
### Argument 3: Number of elements [Integer > 0]

## Examples
                ./main.out vp_softmax matrix.txt 25
                ./main.out vp_sigmoid matrix.txt 25
