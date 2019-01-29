#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int NonLinearActivation(char** argv) {
    string str = argv[1];
    bool relu = str.find("relu") != string::npos;
    ifstream file(argv[2]);
    int r = atoi(argv[3]);
    int c = atoi(argv[4]);

    string a;
    int t = 0;
    vector<float> matrix;
    matrix.reserve(r*c);
    while (getline(file, a)) {
        if (t >= r*c) {
            t -= r*c-1;
        }
        matrix[t] = stof(a);
        t += c;
    }

    if (relu) {
        for(int i = 0; i < r*c; i++) {
            if (matrix[i] < 0) {
                matrix[i] = 0;
            }
        }
    } else {
        for(int i = 0; i < r*c; i++) {
            matrix[i] = tanh(matrix[i]);
        }
    }

    for(int i = 0; i < r; i++) {
        for(int j = 0; j < c; j++) {
            cout << matrix[i*c + j] << "\t";
        }
        cout << "\n";
    }

    return 0;
}
