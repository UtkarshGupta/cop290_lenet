#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/imgcodecs.hpp>

#include "lenet.hpp"
#include "Convolution.hpp"
#include "NonLinearActivation.hpp"
#include "SubSampling.hpp"
#include "VectorProbabilities.hpp"

using namespace std;
typedef vector<float> mat;

void printHelp(int f, int a = -1) {
    if (a == 0) {
        cout << "Incorrect number of arguments, see format below.\n";
    } else if (a > 0) {
        cout << "Error in argument " << a << ", see format below.\n";
    }

    ifstream in("README.md");
    if (!in) {
        cout << "README.md missing.\n";
        return;
    }

    int x, y;
    switch (f) {
        case 0:
                x = 1; y = 12; break;
        case 1:
                x = 14; y = 36; break;
        case 2:
                x = 38; y = 52; break;
        case 3:
                x = 54; y = 64; break;
        case 4:
                x = 66; y = 75; break;
        default:
                x = 1; y = 75; break;
    }

    string l;
    for (int i = 0; i < x - 1; i++) {
        getline(in, l);
    }
    for (int i = x - 1; i < y; i++) {
        getline(in, l);
        cout << l << "\n";
    }
    cout << "\nP.S. All arguments are case-sensitive.\n";

    exit(EXIT_FAILURE);
}

inline int toInt(const char* argv[], int f, int a) {
    string s = argv[a];
    if (s.empty() || s.find_first_not_of("0123456789") != std::string::npos) {
        printHelp(f, a);
    }
    return stoi(s);
}

void readMatrix(const char* path, mat& matrix, int r, int c, int p, bool flip = false) {
    ifstream file (path);
    if (!file) {
        cout << "Cannot open input file: " << path << "\n";
        exit(EXIT_FAILURE);
    } else if (count(istreambuf_iterator<char>(file), istreambuf_iterator<char>(), '\n')
            != (r - 2*p)*(c - 2*p)) {
        cout << "Input file length wrong: " << path << "\n";
        exit(EXIT_FAILURE);
    }
    file.seekg(0);

    int t;
    string l;
    matrix.resize(r*c);
    if (flip) {
        t = (r - p)*c - p - 1;
        while (getline(file, l)) {
            if (t < r*p + p) {
                t += (r - 2*p)*c - 1;
            }
            matrix[t] = stof(l);
            t -= c;
        }
    } else {
        t = r*p + p;
        while (getline(file, l)) {
            if (t >= (r - p)*c) {
                t -= (r - 2*p)*c - 1;
            }
            matrix[t] = stof(l);
            t += c;
        }
    }
}

void printMatrix(mat& matrix, int r, int c) {
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            cout << matrix[i*c + j] << "\t";
        }
        cout << "\n";
    }
}

int main(int argc, char const *argv[]) {
    string arg = argv[1];
    if (arg.find("conv") != string::npos || arg.find("mm") != string::npos) {
        if (argc != 7) {
            printHelp(1, 0);
        }

        bool flip = arg.find("flip") != string::npos;
        int blas;
        if (arg.find("conv") != string::npos) {
            flip = !flip;
            if (arg.find("pthread") != string::npos) {
                blas = 5;
            } else {
                blas = 4;
            }
        } else {
            if (arg.find("pthread") != string::npos) {
                blas = 3;
            } else if (arg.find("openblas") != string::npos) {
                blas = 2;
            } else if (arg.find("intelmkl") != string::npos) {
                blas = 1;
            } else {
                blas = 0;
            }
        }

        int p = toInt(argv, 1, 2);
        int m = toInt(argv, 1, 4) + 2*p;
        int f = toInt(argv, 1, 6);
        int o = m - f + 1;

        mat output, matrix, filter;
        output.resize(o*o);
        if (arg.find("gen") == string::npos) {
            readMatrix(argv[3], matrix, m, m, p);
        } else {
            srand(static_cast <unsigned>(time(0)));
            matrix.resize(m*m, static_cast <float>(rand()) / (static_cast <float>(RAND_MAX/100)));
        }
        readMatrix(argv[5], filter, f, f, 0, flip);

        Convolution(blas, output, matrix, filter);
        if (arg.find("gen") == string::npos) {
            printMatrix(output, o, o);
        }
    } else if (arg.find("ss") != string::npos) {
        if (argc != 6) {
            printHelp(2, 0);
        } else if (arg.find("max") == string::npos && arg.find("avg") == string::npos) {
            printHelp(2, 1);
        }

        int m = toInt(argv, 2, 3);
        int f = toInt(argv, 2, 4);
        int s = toInt(argv, 2, 5);
        int o = (m - f)/s + 1;

        mat output, matrix;
        output.resize(o*o);
        readMatrix(argv[2], matrix, m, m, 0);

        SubSampling(arg.find("max") != string::npos, output, matrix, f, s);
        printMatrix(output, o, o);
    } else if (arg.find("nla") != string::npos) {
        if (argc != 5) {
            printHelp(3, 0);
        } else if (arg.find("relu") == string::npos && arg.find("tanh") == string::npos) {
            printHelp(3, 1);
        }

        int r = toInt(argv, 3, 3);
        int c = toInt(argv, 3, 4);

        mat matrix;
        readMatrix(argv[2], matrix, r, c, 0);

        NonLinearActivation(arg.find("relu") != string::npos, matrix);
        printMatrix(matrix, r, c);
    } else if (arg.find("vp") != string::npos) {
        if (argc != 4) {
            printHelp(4, 0);
        } else if (arg.find("softmax") == string::npos && arg.find("sigmoid") == string::npos) {
            printHelp(4, 1);
        }

        int a = toInt(argv, 4, 3);

        mat array;
        readMatrix(argv[2], array, a, 1, 0);

        VectorProbabilities(arg.find("softmax") != string::npos, array);
        printMatrix(array, a, 1);
    } else if (arg.find("recognize") != string::npos) {
        if (argc != 3) {
            printHelp(0, 0);
        }

        cv::Mat imr = cv::imread(argv[2], cv::IMREAD_GRAYSCALE);
        if (!imr.data) {
            cout << "Cannot open input file: " << argv[2] << "\n";
            exit(EXIT_FAILURE);
        }

        mat image;
        image.assign(imr.datastart, imr.dataend);

        if (arg.find("invert") == string::npos) {
            for (int i = 0; i < 28; i++) {
                for (int j = 0; j < 28; j++) {
                    image[i*28 + j] = 255 - image[i*28 + j];
                }
            }
        }

        lenet(image);
    } else if (arg.find("errorrate") != string::npos) {
        if (argc != 2) {
            printHelp(0, 0);
        }

        lenet(*(new mat));
    } else {
        cout << "Unrecognized command, see format below.\n";
        printHelp(-1);
    }

    exit(EXIT_SUCCESS);
}
