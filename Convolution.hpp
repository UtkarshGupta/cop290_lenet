#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include <vector>

using namespace std;

void Convolution(bool conv, int blas, int s,
        vector<float>& filter, int f, vector<float>& matrix, int m, vector<float>& output, int o);

#endif
