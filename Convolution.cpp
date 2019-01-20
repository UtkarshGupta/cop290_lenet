#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

typedef vector<float> Vector;
typedef vector<vector<float>> Matrix;

void printHelp() {
    ifstream in("./README.md");
    if (!in) {
        cout << "README.md missing.\n";
        return;
    }

    string a;
    for (int i = 0; i <= 18; i++) {
        getline(in, a);
        cout << a << endl;
    }
}

int main(int argc, char** argv) {
    bool padding = false, matrixmult = false;
    int n, m, p = 0;
    string matrix_file, kernel_file;

    // Arguments processing
    if (argc != 7) {
        cout << "Incorrect number Arguments, see format below.\n";
        printHelp();
        return 1;
    }

    string str = argv[1];
    if (str.find("withpadding") != string::npos) {
        padding = true;
    } else if (str.find("withoutpadding") != string::npos) {
        padding = false;
    } else {
        cout << "Error in Argument 1, see format below.\n";
        printHelp();
        return 1;
    }
    if (str.find("matrixmult") != string::npos) {
        matrixmult = true;
    } else if (str.find("convolution") != string::npos) {
        matrixmult = false;
    } else {
        cout << "Error in Argument 1, see format below.\n";
        printHelp();
        return 1;
    }

    if (strtol(argv[2], NULL, 10) != 0) {
        p = atoi(argv[2]);
    } else if (padding) {
        cout << "Error in Argument 2, see format below.\n";
        printHelp();
        return 1;
    }
    if (!padding) {
        p = 0;
    }

    if (strtol(argv[3], NULL, 10) == 0) {
        kernel_file = argv[3];
    } else {
        cout << "Error in Argument 3, see format below.\n";
        printHelp();
        return 1;
    }

    if (strtol(argv[4], NULL, 10) != 0) {
        m = atoi(argv[4]);
    } else {
        cout << "Error in Argument 4, see format below.\n";
        printHelp();
        return 1;
    }

    if (strtol(argv[5], NULL, 10) == 0) {
        matrix_file = argv[5];
    } else {
        cout << "Error in Argument 5, see format below.\n";
        printHelp();
        return 1;
    }

    if (strtol(argv[6], NULL, 10) != 0) {
        n = atoi(argv[6]);
    } else {
        cout << "Error in Argument 6, see format below.\n";
        printHelp();
        return 1;
    }

    // Input processing
    ifstream kernelf(kernel_file);
    ifstream matrixf(matrix_file);

    if (!kernelf) {
        cout << "Cannot open input file: " << kernel_file << ".\nSee input format below.\n";
        printHelp();
        return 1;
    } else if (count(istreambuf_iterator<char>(kernelf), istreambuf_iterator<char>(), '\n') != m*m) {
        cout << "Input file length wrong.\n" << "See input format below.\n";
        printHelp();
        return 1;
    }
    kernelf.clear();
    kernelf.seekg(0);
    if (!matrixf) {
        cout << "Cannot open input file: " << matrix_file << ".\nSee input format below.\n";
        printHelp();
        return 1;
    } else if (count(istreambuf_iterator<char>(matrixf), istreambuf_iterator<char>(), '\n') != n*n) {
        cout << "Input file length wrong.\n" << "See input format below.\n";
        printHelp();
        return 1;
    }
    matrixf.clear();
    matrixf.seekg(0);

    string a;
    int i = 0, j = 0;
    Matrix kernel;
    kernel.resize(m, vector<float>(m, 0.0));
    while (getline(kernelf, a)) {
        if (i == m) {
            i = 0;
            j++;
        }
        kernel[i++][j] = stof(a);
    }

    i = p; j = p;
    Matrix matrix;
    matrix.resize(n + 2*p, vector<float>(n + 2*p, 0.0));
    while (getline(matrixf, a)) {
        if (i == n + p) {
            i = p;
            j++;
        }
        matrix[i++][j] = stof(a);
    }

    // Showing input
    cout << "Input Kernel: \n";
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < m; j++) {
            cout << kernel[i][j] << "\t";
        }
        cout << "\n";
    }
    cout << "Input Matrix: \n";
    for(int i = p; i < n + p; i++) {
        for(int j = p; j < n + p; j++) {
            cout << matrix[i][j] << "\t";
        }
        cout << "\n";
    }
    if (matrixmult) {
        cout << "Using Matrix Multiplication.\n";
    } else {
        cout << "Using Convolution.\n";
    }

    if (padding) {
        cout << "Padding: Enabled.\n";
        cout << "Padding size: " << p << "\n";
    } else {
        cout << "Padding: Disabled.\n";
    }
    cout << "\n";

    // Processing
    int output_size = n - m + 2*p + 1;
    Matrix output;
    output.resize(output_size, vector<float>(output_size, 0.0));
    if (matrixmult) {
        n = n + 2*p;
        int rows = pow(output_size, 2);
        int cols = pow(n, 2);
        int skipped = 0;

        Vector x;
        x.resize(cols);
        Matrix sparse;
        sparse.resize(rows, vector<float>(cols, 0.0));

        for (int i = 0; i < cols; i++) {
            x[i] = matrix[i/n][i%n];
        }
        for (int i = 0; i < cols; i++) {
            if ((i % n) < output_size && ((i / n) % n) < output_size) {
                for (int j = 0; j < m; j++) {
                    for (int k = 0; k < m; k++) {
                        sparse[i - skipped][i + j * n + k] = kernel[j][k];
                    }
                }
            } else {
                skipped++;
            }
        }

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                output[i/output_size][i%output_size] += sparse[i][j]*x[j];
            }
        }
    } else {
        int r, c;
        for(int x = 0; x < output_size; x++) {
            for(int y = 0; y < output_size; y++) {
                for (int u = 0; u < m; u++) {
                    for (int v = 0; v < m; v++) {
                        r = x-u+1;
                        c = y-v+1;
                        if (r >= 0 && r < n+2*p && r >= 0 && c < n+2*p) {
                            output[x][y] += kernel[u][v] * matrix[r][c];
                        }
                    }
                }
            }
        }
    }

    // Output
    cout << "Output: \n";
    for(int i = 0; i < output_size; i++) {
        for(int j = 0; j < output_size; j++) {
            cout << output[i][j] << "\t";
        }
        cout << "\n";
    }
}
