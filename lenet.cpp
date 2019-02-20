#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Convolution.hpp"
#include "NonLinearActivation.hpp"
#include "SubSampling.hpp"
#include "VectorProbabilities.hpp"

#define BLAS 4

using namespace std;
typedef vector<float> mat;

void readFile(string path, vector<mat>& filters, mat& filtersb, int n, int m, int s) {
    ifstream file (path);
    if (!file) {
        cout << "Cannot open file: " << path << "\n";
        exit(EXIT_FAILURE);
    }

    filters.resize(n*m);
    filtersb.resize(n);

    string l;
    mat filter (s*s);
    for (int i = 0; i < n*m; i++) {
        for (int j = 0; j < s*s; j++) {
            getline(file, l);
            filter[j] = stof(l);
        }
        filters[i] = filter;
    }

    for (int i = 0; i < n; i++) {
        getline(file, l);
        filtersb[i] = stof(l);
    }
}

void lenet(mat& image) {
    mat conv1b, conv2b, fc1b, fc2b;
    vector<mat> conv1f, conv2f, fc1f, fc2f;

    readFile("conv1.txt", conv1f, conv1b, 20, 1, 5);
    readFile("conv2.txt", conv2f, conv2b, 50, 20, 5);
    readFile("fc1.txt", fc1f, fc1b, 500, 50, 4);
    readFile("fc2.txt", fc2f, fc2b, 10, 500, 1);

    mat conv1o(24*24), pool1o(12*12), conv2o(8*8), pool2o(4*4);
    mat relu(500), softmax(10);
    vector<mat> pool1(20), pool2(50);

    for (int i = 0; i < 20; i++) {
        fill(conv1o.begin(), conv1o.end(), conv1b[i]);
        Convolution(BLAS, conv1o, image, conv1f[i]);

        SubSampling(true, pool1o, conv1o, 2, 2);
        pool1[i] = pool1o;
    }

    for (int i = 0; i < 50; i++) {
        fill(conv2o.begin(), conv2o.end(), conv2b[i]);
        for (int j = 0; j < 20; j++) {
            Convolution(BLAS, conv2o, pool1[j], conv2f[i*20 + j]);
        }

        SubSampling(true, pool2o, conv2o, 2, 2);
        pool2[i] = pool2o;
    }

    for (int i = 0; i < 500; i++) {
        relu[i] = fc1b[i];
        for (int j = 0; j < 50; j++) {
            for (int k = 0; k < 16; k++) {
                relu[i] += pool2[j][k]*fc1f[i*50 + j][k];
            }
        }
    }
    NonLinearActivation(true, relu);

    for (int i = 0; i < 10; i++) {
        softmax[i] = fc2b[i];
        for (int j = 0; j < 500; j++) {
            softmax[i] += relu[j]*fc2f[i*500 + j][0];
        }
    }
    VectorProbabilities(true, softmax);

    for (int i = 0; i < 10; i++) {
        if (softmax[i] > 0.01) {
            cout << i << " " << softmax[i]*100 << "%\n";
        }
    }
}
