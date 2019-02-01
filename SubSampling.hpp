#ifndef SUBSAMPLING_H
#define SUBSAMPLING_H

#include <vector>

using namespace std;

void SubSampling(bool max, int f, int s,
        vector<float>& matrix, int m, vector<float>& output, int o);

#endif
