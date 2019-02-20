#include <cmath>
#include <vector>

typedef std::vector<float> mat;

void SubSampling(bool max, mat& output, mat& matrix, int f, int s) {
    int o = sqrt(output.size());
    int m = sqrt(matrix.size());
    if (max) {
        for (int i = 0; i < o*s; i+=s) {
            for (int j = 0; j < o*s; j+=s) {
                output[i/s*o + j/s] = matrix[i*m + j];
                for (int x = i; x < i+f; x++) {
                    for (int y = j; y < j+f; y++) {
                        output[i/s*o + j/s] = fmax(output[i/s*o + j/s], matrix[x*m + y]);
                    }
                }
            }
        }
    } else {
        for (int i = 0; i < o*s; i+=s) {
            for (int j = 0; j < o*s; j+=s) {
                for (int x = i; x < i+f; x++) {
                    for (int y = j; y < j+f; y++) {
                        output[i/s*o + j/s] += matrix[x*m + y]/(f*f);
                    }
                }
            }
        }
    }
}
