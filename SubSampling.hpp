#ifndef SUBSAMPLING_H
#define SUBSAMPLING_H

#include <vector>

typedef std::vector<float> mat;

void SubSampling(bool max, mat& output, mat& matrix, int f, int s);

#endif
