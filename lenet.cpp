#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "Convolution.hpp"
#include "NonLinearActivation.hpp"
#include "SubSampling.hpp"
#include "VectorProbabilities.hpp"

#define BLAS 4

using namespace std;
typedef vector<float> mat;

void readFile(string path, vector<mat>& filters, mat& filtersb, int n, int s) {
    ifstream file (path);
    if (!file) {
        cout << "Cannot open file: " << path << "\n";
        exit(EXIT_FAILURE);
    }

    int t;
    string l;
    mat filter (s*s);
    for (int i = 0; i < n; i++) {
        t = 0;
        while (getline(file, l)) {
            filter[t++] = stof(l);
            if (t == s*s) {
                break;
            }
        }
        filters.push_back(filter);
    }

    while (getline(file, l)) {
        filtersb.push_back(stof(l));
    }
}

inline int reverseInt(int i) {
    return ((i & 255) << 24) + (((i >> 8) & 255) << 16) + (((i >> 16) & 255) << 8) + ((i >> 24) & 255);
}

void readMNISTImages(string path, vector<mat> &images) {
    ifstream file (path, ios::binary);
    if (!file) {
        cout << "Cannot open file: " << path << "\n";
        exit(EXIT_FAILURE);
    }

    int m, n, r, c;
    file.read((char*) &m, sizeof(m));
    m = reverseInt(m);
    file.read((char*) &n, sizeof(n));
    n = reverseInt(n);
    file.read((char*) &r, sizeof(r));
    r = reverseInt(r);
    file.read((char*) &c, sizeof(c));
    c = reverseInt(c);

    unsigned char pixel;
    mat image;
    for (int i = 0; i < n; i++) {
        image.clear();
        for (int j = 0; j < r*c; j++) {
            file.read((char*) &pixel, sizeof(pixel));
            image.push_back((float) pixel);
        }
        images.push_back(image);
    }
}

void readMNISTLabels(string path, mat &labels) {
    ifstream file (path, ios::binary);
    if (!file) {
        cout << "Cannot open file: " << path << "\n";
        exit(EXIT_FAILURE);
    }

    int m, n;
    file.read((char*) &m, sizeof(m));
    m = reverseInt(m);
    file.read((char*) &n, sizeof(n));
    n = reverseInt(n);

    unsigned char label;
    for (int i = 0; i < n; i++) {
        file.read((char*) &label, sizeof(label));
        labels.push_back((float) label);
    }
}

mat conv1b, conv2b, fc1b, fc2b;
vector<mat> conv1f, conv2f, fc1f, fc2f;
mat conv1o(24*24), pool1o(12*12), conv2o(8*8), pool2o(4*4);
mat relu(500), softmax(10);
vector<mat> pool1(20), pool2(50);

mat& operator+=(mat& m, float x) {
    for (auto e = begin(m); e != end(m); ++e) {
        *e += x;
    }
    return m;
}

mat& operator+=(mat& a, mat& b) {
    transform (a.begin(), a.end(), b.begin(), a.begin(), plus<float>());
    return a;
}

int recognize(mat& image, int n = -1) {
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
            relu[i] += inner_product(pool2[j].begin(), pool2[j].end(), fc1f[i*50 + j].begin(), 0);
        }
    }
    NonLinearActivation(true, relu);

    for (int i = 0; i < 10; i++) {
        softmax[i] = fc2b[i];
        for (int j = 0; j < 500; j++) {
            softmax[i] += relu[j] * fc2f[i*500 + j][0];
        }
    }
    VectorProbabilities(true, softmax);

    if (n == -1) {
        cout << fixed;
        cout << setprecision(2);
        for (int i = 0; i < 10; i++) {
            if (softmax[i] > 0) {
                cout << i << " " << softmax[i]*100 << "%\n";
            }
        }
    } else {
        if (softmax[n] < 0.95) {
            return 1;
        }
    }
    return 0;
}

void lenet(mat image) {
    readFile("conv1.txt", conv1f, conv1b, 20*1, 5);
    readFile("conv2.txt", conv2f, conv2b, 50*20, 5);
    readFile("fc1.txt", fc1f, fc1b, 500*50, 4);
    readFile("fc2.txt", fc2f, fc2b, 10*500, 1);

    if (image.size() != 0) {
        recognize(image);
    } else {
        mat labels;
        readMNISTLabels("t10k-labels.idx1-ubyte", labels);
        vector<mat> images;
        readMNISTImages("t10k-images.idx3-ubyte", images);

        float c = 0;
        for (int i = 0; i < 10000; i++) {
            c += recognize(images[i], labels[i]);
        }
        cout << "Error rate: " << c/100 << "%\n";
    }
}
