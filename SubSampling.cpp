#include <cmath>
#include <vector>

typedef std::vector<float> mat;

void SubSampling(bool max, mat& output, mat& matrix, int f, int s) {
    int o = sqrt(output.size());
    int m = sqrt(matrix.size());
    float t;
    if (max) {
        for (int i = 0; i < o*s; i+=s) {
            for (int j = 0; j < o*s; j+=s) {
                t = matrix[i*m + j];
                for (int x = i; x < i+f; x++) {
                    for (int y = j; y < j+f; y++) {
                        t = fmax(t, matrix[x*m + y]);
                    }
                }
                output[i/s*o + j/s] = t;
            }
        }
    } else {
        for (int i = 0; i < o*s; i+=s) {
            for (int j = 0; j < o*s; j+=s) {
                t = 0;
                for (int x = i; x < i+f; x++) {
                    for (int y = j; y < j+f; y++) {
                        t += matrix[x*m + y];
                    }
                }
                output[i/s*o + j/s] = t/(f*f);
            }
        }
    }
}
