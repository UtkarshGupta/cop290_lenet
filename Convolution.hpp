#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include <vector>
typedef std::vector<float> mat;

void Convolution(int blas, mat& output, mat& matrix, mat& filter);

#endif
