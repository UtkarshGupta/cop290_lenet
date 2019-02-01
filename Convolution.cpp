#include <cmath>
#include <iostream>
#include <vector>

namespace openblas {
    #include <cblas.h>
}
namespace intelmkl {
    #include <mkl.h>
}

using namespace std;

#define THREADS 8

vector<float> filterg, outputg, toeplitz;

void* func(void *tid) {
    long id = (long) tid;
    int r = outputg.capacity();
    int c = filterg.capacity();

    for (int i = id*r/THREADS; i < (id+1)*r/THREADS; i++) {
        for (int j = 0; j < c; j++) {
            outputg[i] += toeplitz[i*c + j]*filterg[j];
        }
    }

    pthread_exit(NULL);
}

void Convolution(bool conv, int blas, int s,
        vector<float>& filter, int f, vector<float>& matrix, int m, vector<float>& output, int o) {
    if (conv) {
        for(int i = 0; i < o * s; i+=s) {
            for(int j = 0; j < o * s; j+=s) {
                for (int x = i; x < i + f; x++) {
                    for (int y = j; y < j + f; y++) {
                        output[i/s*o + j/s] += filter[(x-i)*f + y-j] * matrix[x*m + y];
                    }
                }
            }
        }
    } else {
        int r = pow(o, 2);
        int c = pow(f, 2);
        toeplitz.reserve(r*c);

        int t = 0;
        for(int i = 0; i < o * s; i+=s) {
            for(int j = 0; j < o * s; j+=s) {
                for (int x = i; x < i + f; x++) {
                    for (int y = j; y < j + f; y++) {
                        toeplitz[t++] = matrix[x*m + y];
                    }
                }
            }
        }

        if (blas == -1) {
            for (int i = 0; i < r; i++) {
                for (int j = 0; j < c; j++) {
                    output[i] += toeplitz[i*c + j]*filter[j];
                }
            }
        } else if (blas == 0) {
            filterg.swap(filter);
            outputg.swap(output);

            pthread_t t[THREADS];
            for (int i = 0; i < THREADS; i++) {
                pthread_create(&t[i], NULL, func, (void *) (intptr_t)i);
            }
            for (int i = 0; i < THREADS; i++) {
                pthread_join(t[i], NULL);
            }

            filter.swap(filterg);
            output.swap(outputg);
        } else if (blas == 1) {
            openblas::cblas_sgemv(openblas::CblasRowMajor, openblas::CblasNoTrans,
                    r, c, 1, &toeplitz[0], c, &filter[0], 1, 0, &output[0], 1);
        } else if (blas == 2) {
            intelmkl::cblas_sgemv(intelmkl::CblasRowMajor, intelmkl::CblasNoTrans,
                    r, c, 1, &toeplitz[0], c, &filter[0], 1, 0, &output[0], 1);
        }
    }
}
