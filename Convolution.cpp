#include <cmath>
#include <iostream>
#include <vector>

namespace openblas {
    #include <cblas.h>
}
namespace intelmkl {
    #include <mkl.h>
}

#define THREADS 8
typedef std::vector<float> mat;

mat toeplitz, outputg, matrixg, filterg;

void* conv(void *tid) {
    long id = (long) tid;
    int o = sqrt(outputg.size());
    int m = sqrt(matrixg.size());
    int f = sqrt(filterg.size());

    for (int i = id*o/THREADS; i < (id+1)*o/THREADS; i++) {
        for (int j = 0; j < o; j++) {
            for (int x = i; x < i+f; x++) {
                for (int y = j; y < j+f; y++) {
                    outputg[i*o + j] += filterg[(x-i)*f + y-j] * matrixg[x*m + y];
                }
            }
        }
    }

    pthread_exit(NULL);
}

void* mm(void *tid) {
    long id = (long) tid;
    int r = outputg.size();
    int c = filterg.size();

    for (int i = id*r/THREADS; i < (id+1)*r/THREADS; i++) {
        for (int j = 0; j < c; j++) {
            outputg[i] += toeplitz[i*c + j]*filterg[j];
        }
    }

    pthread_exit(NULL);
}

void Convolution(int blas, mat& output, mat& matrix, mat& filter) {
    int o = sqrt(output.size());
    int m = sqrt(matrix.size());
    int f = sqrt(filter.size());

    if (blas == 5) {
        outputg.swap(output);
        matrixg.swap(matrix);
        filterg.swap(filter);

        pthread_t t[THREADS];
        for (int i = 0; i < THREADS; i++) {
            pthread_create(&t[i], NULL, conv, (void *) (intptr_t)i);
        }
        for (int i = 0; i < THREADS; i++) {
            pthread_join(t[i], NULL);
        }

        output.swap(outputg);
        matrix.swap(matrixg);
        filter.swap(filterg);
    } else if (blas == 4) {
        for (int i = 0; i < o; i++) {
            for (int j = 0; j < o; j++) {
                for (int x = i; x < i+f; x++) {
                    for (int y = j; y < j+f; y++) {
                        output[i*o + j] += filter[(x-i)*f + y-j] * matrix[x*m + y];
                    }
                }
            }
        }
    } else {
        int r = pow(o, 2);
        int c = pow(f, 2);
        toeplitz.resize(r*c);

        int t = 0;
        for (int i = 0; i < o; i++) {
            for (int j = 0; j < o; j++) {
                for (int x = i; x < i+f; x++) {
                    for (int y = j; y < j+f; y++) {
                        toeplitz[t++] = matrix[x*m + y];
                    }
                }
            }
        }

        if (blas == 3) {
            outputg.swap(output);
            filterg.swap(filter);

            pthread_t t[THREADS];
            for (int i = 0; i < THREADS; i++) {
                pthread_create(&t[i], NULL, mm, (void *) (intptr_t)i);
            }
            for (int i = 0; i < THREADS; i++) {
                pthread_join(t[i], NULL);
            }

            output.swap(outputg);
            filter.swap(filterg);
        } else if (blas == 2) {
            openblas::cblas_sgemv(openblas::CblasRowMajor, openblas::CblasNoTrans,
                    r, c, 1, &toeplitz[0], c, &filter[0], 1, 0, &output[0], 1);
        } else if (blas == 1) {
            intelmkl::cblas_sgemv(intelmkl::CblasRowMajor, intelmkl::CblasNoTrans,
                    r, c, 1, &toeplitz[0], c, &filter[0], 1, 0, &output[0], 1);
        } else {
            for (int i = 0; i < r; i++) {
                for (int j = 0; j < c; j++) {
                    output[i] += toeplitz[i*c + j]*filter[j];
                }
            }
        }
    }
}
