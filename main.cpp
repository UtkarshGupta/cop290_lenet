#include <algorithm>
#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Convolution.hpp"
#include "NonLinearActivation.hpp"
#include "SubSampling.hpp"
#include "VectorProbabilities.hpp"

using namespace std;

inline bool isNumber(const char* c) {
    string s = c;
    return !s.empty() && s.find_first_not_of("0123456789") == std::string::npos;
}

void printHelp(int f, int arg = -1) {
    if (arg == 0) {
        cout << "Incorrect number of arguments, see format below.\n";
    } else if (arg > 0) {
        cout << "Error in argument " << arg << ", see format below.\n";
    }

    ifstream in("./README.md");
    if (!in) {
        cout << "README.md missing.\n";
        return;
    }

    int a, b;
    switch(f) {
        case 1:
                a = 1;
                b = 21;
                break;
        case 2:
                a = 23;
                b = 37;
                break;
        case 3:
                a = 39;
                b = 49;
                break;
        case 4:
                a = 51;
                b = 61;
                break;
        default:
                a = 1;
                b = 61;
                break;
    }

    string l;
    for (int i = 0; i < a - 1; i++) {
        getline(in, l);
    }
    for (int i = a - 1; i < b; i++) {
        getline(in, l);
        cout << l << "\n";
    }
    cout << "\nP.S. All arguments are case-sensitive.\n";

    exit(EXIT_FAILURE);
}

void checkFile(const char* path, int r, int c = 1) {
    ifstream file (path);
    if (!file) {
        cout << "Cannot open input file: " << path << "\n";;
        exit(EXIT_FAILURE);
    } else if (count(istreambuf_iterator<char> (file), istreambuf_iterator<char> (), '\n') != r*c) {
        cout << "Input file length wrong: " << path << "\n";
        exit(EXIT_FAILURE);
    }
}

void readMatrix(vector<float>& matrix, const char* path, int r, int c, int p, bool flip = false) {
    ifstream file (path);
    string l;
    int t;
    if (flip) {
        t = r*c - 1;
        while (getline(file, l)) {
            if (t < 0) {
                t += r*c - 1;
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

void printMatrix(vector<float>& matrix, int r, int c) {
    for(int i = 0; i < r; i++) {
        for(int j = 0; j < c; j++) {
            cout << matrix[i*c + j] << "\t";
        }
        cout << "\n";
    }
}

void library(int argc, char const *argv[]) {
    string str = argv[1];
    if (str.find("conv") != string::npos || str.find("mm") != string::npos) {
        if (argc != 8) {
            printHelp(1, 0);
        }
        bool conv = str.find("conv") != string::npos;
        bool flip = conv ? str.find("flip") == string::npos : str.find("flip") != string::npos;

        if (!isNumber(argv[2])) {
            printHelp(1, 2);
        }
        int p = atoi(argv[2]);

        if (!isNumber(argv[3])) {
            printHelp(1, 3);
        }
        int s = atoi(argv[3]);

        if (!isNumber(argv[5])) {
            printHelp(1, 5);
        }
        int f = atoi(argv[5]);

        if (!isNumber(argv[7])) {
            printHelp(1, 7);
        }
        int m = atoi(argv[7]);

        if (isNumber(argv[4])) {
            printHelp(1, 4);
        } else {
            checkFile(argv[4], f, f);
        }
        vector<float> filter;
        filter.reserve(f*f);
        readMatrix(filter, argv[4], f, f, 0, flip);

        vector<float> matrix;
        if (isNumber(argv[6])) {
            printHelp(1, 6);
        } else if (str.find("gen") == string::npos) {
            checkFile(argv[6], m, m);
            m = m + 2*p;
            matrix.reserve(m*m);
            readMatrix(matrix, argv[6], m, m, p);
        } else {
            m = m + 2*p;
            srand(static_cast <unsigned> (time(0)));
            matrix.resize(m*m, static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/100)));
        }

        int blas = -1;
        if (str.find("pthread") != string::npos) {
            blas = 0;
        } else if (str.find("openblas") != string::npos) {
            blas = 1;
        } else if (str.find("intelmkl") != string::npos) {
            blas = 2;
        }
        int o = (m - f)/s + 1;
        vector<float> output;
        output.reserve(o*o);

        Convolution(conv, blas, s, filter, f, matrix, m, output, o);
        if (str.find("gen") == string::npos) {
            printMatrix(output, o, o);
        }
    } else if (str.find("ss") != string::npos) {
        if (argc != 6) {
            printHelp(2, 0);
        }

        if (str.find("max") == string::npos && str.find("avg") == string::npos) {
            printHelp(2, 1);
        }

        if (!isNumber(argv[3])) {
            printHelp(2, 3);
        }
        int m = atoi(argv[3]);

        if (!isNumber(argv[4])) {
            printHelp(2, 4);
        }
        int f = atoi(argv[4]);

        if (!isNumber(argv[5])) {
            printHelp(2, 5);
        }
        int s = atoi(argv[5]);

        if (isNumber(argv[2])) {
            printHelp(2, 2);
        } else {
            checkFile(argv[2], m, m);
        }
        vector<float> matrix;
        matrix.reserve(m*m);
        readMatrix(matrix, argv[2], m, m, 0);

        int o = (m - f)/s + 1;
        vector<float> output;
        output.reserve(o*o);

        SubSampling(str.find("max") != string::npos, f, s, matrix, m, output, o);
        printMatrix(output, o, o);
    } else if (str.find("nla") != string::npos) {
        if (argc != 5) {
            printHelp(3, 0);
        }

        if (str.find("relu") == string::npos && str.find("tanh") == string::npos) {
            printHelp(3, 1);
        }

        if (!isNumber(argv[3])) {
            printHelp(3, 3);
        }
        int r = atoi(argv[3]);

        if (!isNumber(argv[4])) {
            printHelp(3, 4);
        }
        int c = atoi(argv[4]);

        if (isNumber(argv[2])) {
            printHelp(3, 2);
        } else {
            checkFile(argv[2], r, c);
        }
        vector<float> matrix;
        matrix.reserve(r*c);
        readMatrix(matrix, argv[2], r, c, 0);

        NonLinearActivation(str.find("relu") != string::npos, matrix, r, c);
        printMatrix(matrix, r, c);
    } else if (str.find("vp") != string::npos) {
        if (argc != 4) {
            printHelp(4, 0);
        }

        if (str.find("sigmoid") == string::npos && str.find("softmax") == string::npos) {
            printHelp(4, 1);
        }

        if (!isNumber(argv[3])) {
            printHelp(4, 3);
        }
        int a = atoi(argv[3]);

        if (isNumber(argv[2])) {
            printHelp(4, 2);
        } else {
            checkFile(argv[2], a);
        }
        vector<float> array;
        array.reserve(a);
        readMatrix(array, argv[2], a, 1, 0);

        VectorProbabilities(str.find("sigmoid") != string::npos, array, a);
        printMatrix(array, a, 1);
    } else {
        cout << "Unrecognized command, see format below.\n";
        printHelp(0);
    }
}

vector<float>& operator+=(vector<float>& v, float x) {
	for (auto a = begin(v); a != end(v); ++a) {
		*a += x;
	}
	return v;
}

vector<float>& operator*=(vector<float>& v, float x) {
	for (auto a = begin(v); a != end(v); ++a) {
		*a *= x;
	}
	return v;
}

int main(int argc, char const *argv[]) {
    if (argc > 3) {
        library(argc, argv);
        return 0;
    }

    return 0;
}
