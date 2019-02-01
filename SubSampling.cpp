#include <cmath>
#include <vector>

using namespace std;

void SubSampling(bool max, int f, int s,
        vector<float>& matrix, int m, vector<float>& output, int o) {
    float t = 0;
    if (max) {
        for(int i = 0; i < o * s; i+=s) {
            for(int j = 0; j < o * s; j+=s) {
                for (int x = i; x < i + f; x++) {
                    for (int y = j; y < j + f; y++) {
                        t = fmax(t, matrix[x*m + y]);
                    }
                }
                output[i/s*o + j/s] = t;
                t = 0;
            }
        }
    } else {
        for(int i = 0; i < o * s; i+=s) {
            for(int j = 0; j < o * s; j+=s) {
                for (int x = i; x < i + f; x++) {
                    for (int y = j; y < j + f; y++) {
                        t += matrix[x*m + y];
                    }
                }
                output[i/s*o + j/s] = t/(f*f);
                t = 0;
            }
        }
    }
}
