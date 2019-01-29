#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace openblas {
    #include <cblas.h>
}
namespace intelmkl {
    #include <mkl.h>
}

using namespace std;

#define THREADS 8

vector<float> kernel, matrix, output, sparse;

void* func(void *tid) {
    long id = (long) tid;
    int r = output.capacity();
    int c = matrix.capacity();

    for (int i = id*r/THREADS; i < (id+1)*r/THREADS; i++) {
        for (int j = 0; j < c; j++) {
            output[i] += sparse[i*c + j]*matrix[j];
        }
    }

    pthread_exit(NULL);
}

int Convolution(char** argv) {
    string str = argv[1];
    bool matrixmult = str.find("matrixmult") != string::npos;
    bool flip = str.find("flip") != string::npos;
    int blas = -1;
    if (str.find("pthread") != string::npos) {
        blas = 0;
    } else if (str.find("openblas") != string::npos) {
        blas = 1;
    } else if (str.find("intelmkl") != string::npos) {
        blas = 2;
    } else if (str.find("quick") != string::npos) {
        blas = 3;
    }
    int p = atoi(argv[2]);
    ifstream kernelf (argv[3]);
    int m = atoi(argv[4]);
    ifstream matrixf (argv[5]);
    int n = atoi(argv[6]) + 2*p;
    int o = n - m + 1;

    string a;
    int t;
    kernel.reserve(m*m);
    if (flip) {
        t = m*m - 1;
        while (getline(kernelf, a)) {
            if (t < 0) {
                t += m*m - 1;
            }
            kernel[t] = stof(a);
            t -= m;
        }
    } else {
        t = 0;
        while (getline(kernelf, a)) {
            if (t >= m*m) {
                t -= m*m - 1;
            }
            kernel[t] = stof(a);
            t += m;
        }
    }
    t = n*p + p;
    matrix.reserve(n*n);
    while (getline(matrixf, a)) {
        if (t >= (n - p)*n) {
            t -= (n - 2*p)*n - 1;
        }
        matrix[t] = stof(a);
        t += n;
    }

    int r, c;
    output.reserve(o*o);
    if (matrixmult) {
        r = pow(o, 2);
        c = pow(n, 2);
        sparse.reserve(r*c);

        t = 0;
        for (int i = 0; i < c; i++) {
            if ((i % n) < o && ((i / n) % n) < o) {
                for (int j = 0; j < m; j++) {
                    for (int k = 0; k < m; k++) {
                        sparse[(i - t)*c + i + j*n + k] = kernel[j*m + k];
                    }
                }

                if (blas == 3) {
                    for (int j = (i - t)*c + i; j <= (i - t)*c + i + (m-1)*(n+1); j++) {
                        output[i - t] += sparse[j]*matrix[j%c];
                    }
                }
            } else {
                t++;
            }

            if (blas == -1) {
                for (int j = 0; j < r; j++) {
                    output[j] += sparse[j*c + i]*matrix[i];
                }
            }
        }

        if (blas == 0) {
            pthread_t t[THREADS];
            for (int i = 0; i < THREADS; i++) {
                pthread_create(&t[i], NULL, func, (void *) (intptr_t)i);
            }
            for (int i = 0; i < THREADS; i++) {
                pthread_join(t[i], NULL);
            }
        } else if (blas == 1) {
            openblas::cblas_sgemv(openblas::CblasRowMajor, openblas::CblasNoTrans,
                    r, c, 1, &sparse[0], c, &matrix[0], 1, 0, &output[0], 1);
        } else if (blas == 2) {
            intelmkl::cblas_sgemv(intelmkl::CblasRowMajor, intelmkl::CblasNoTrans,
                    r, c, 1, &sparse[0], c, &matrix[0], 1, 0, &output[0], 1);
        }
    } else {
        for(int x = 0; x < o; x++) {
            for(int y = 0; y < o; y++) {
                for (int u = 0; u < n; u++) {
                    for (int v = 0; v < n; v++) {
                        r = x - u + m - 1;
                        c = y - v + m - 1;
                        if (r >= 0 && r < m && c >= 0 && c < m) {
                            output[x*o + y] += kernel[r*m + c]*matrix[u*n + v];
                        }
                    }
                }
            }
        }
    }

    for(int i = 0; i < o; i++) {
        for(int j = 0; j < o; j++) {
            cout << output[i*o + j] << "\t";
        }
        cout << "\n";
    }

    return 0;
}
