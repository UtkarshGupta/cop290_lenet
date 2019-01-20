#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

typedef vector<vector<float>> Matrix;

void printHelp() {
    ifstream in("./README.md");
    if (!in) {
        cout << "README.md missing.\n";
        return;
    }

    string a;
    for (int i = 0; i <= 19; i++) {
        getline(in, a);
    }
    for (int i = 20; i <= 30; i++) {
        getline(in, a);
        cout << a << endl;
    }
}

int main(int argc, char** argv) {
    bool relu;
    int rows, cols;
    string file;

    // Arguments processing
    if (argc != 5) {
        cout << "Incorrect number Arguments, see format below.\n";
        printHelp();
        return 1;
    }

    string str = argv[1];
    if (str.find("relu") != string::npos) {
        relu = true;
    } else if (str.find("tanh") != string::npos) {
        relu = false;
    } else {
        cout << "Error in Argument 1, see format below.\n";
        printHelp();
        return 1;
    }

    if (strtol(argv[2], NULL, 10) == 0) {
        file = argv[2];
    } else {
        cout << "Error in Argument 2, see format below.\n";
        printHelp();
        return 1;
    }

    if (strtol(argv[3], NULL, 10) != 0) {
        rows = atoi(argv[3]);
    } else {
        cout << "Error in Argument 3, see format below.\n";
        printHelp();
        return 1;
    }

    if (strtol(argv[4], NULL, 10) != 0) {
        cols = atoi(argv[4]);
    } else {
        cout << "Error in Argument 3, see format below.\n";
        printHelp();
        return 1;
    }

    // Input processing
    ifstream matrixf(file);
    if (!matrixf) {
        cout << "Cannot open input file: " << file << ".\nSee input format below.\n";
        printHelp();
        return 1;
    } else if (count(istreambuf_iterator<char>(matrixf), istreambuf_iterator<char>(), '\n') != rows*cols) {
        cout << "Input file length wrong.\n" << "See input format below.\n";
        printHelp();
        return 1;
    }
    matrixf.clear();
    matrixf.seekg(0);

    string a;
    int i = 0, j = 0;
    Matrix matrix;
    matrix.resize(rows, vector<float>(cols, 0.0));
    while (getline(matrixf, a)) {
        if (i == rows) {
            i = 0;
            j++;
        }
        matrix[i++][j] = stof(a);
    }

    // Showing input
    cout << "Input Matrix: \n";
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            cout << matrix[i][j] << "\t";
        }
        cout << "\n";
    }
    if (relu) {
        cout << "Using relu.\n\n";
    } else {
        cout << "Using tanh.\n\n";
    }

    // Processing
    if (relu) {
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                if (matrix[i][j] < 0) {
                    matrix[i][j] = 0;
                }
            }
        }
    } else {
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                matrix[i][j] = tanh(matrix[i][j]);
            }
        }
    }

    // Output
    cout << "Output: \n";
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            cout << matrix[i][j] << "\t";
        }
        cout << "\n";
    }
}
