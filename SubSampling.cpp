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
    for (int i = 0; i <= 31; i++) {
        getline(in, a);
    }
    for (int i = 32; i <= 47; i++) {
        getline(in, a);
        cout << a << endl;
    }
}

int main(int argc, char** argv) {
    bool maxpooling;
    int rows, filter, stride, size;
    string file;

    // Arguments processing
    if (argc != 6) {
        cout << "Incorrect number Arguments, see format below.\n";
        printHelp();
        return 1;
    }

    string str = argv[1];
    if (str.find("maxpooling") != string::npos) {
        maxpooling = true;
    } else if (str.find("avgpooling") != string::npos) {
        maxpooling = false;
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
        filter = atoi(argv[4]);
    } else {
        cout << "Error in Argument 4, see format below.\n";
        printHelp();
        return 1;
    }

    if (strtol(argv[5], NULL, 10) != 0) {
        stride = atoi(argv[5]);
        //if (size < stride && (rows - filter)/stride + 1) == floor((rows - filter)/stride + 1)
        size = (rows - filter)/stride + 1;
        cout << size;
    } else {
        cout << "Error in Argument 5, see format below.\n";
        printHelp();
        return 1;
    }

    // Input processing
    ifstream inputf(file);
    if (!inputf) {
        cout << "Cannot open input file: " << file << ".\nSee input format below.\n";
        printHelp();
        return 1;
    } else if (count(istreambuf_iterator<char>(inputf), istreambuf_iterator<char>(), '\n') != rows*rows) {
        cout << "Input file length wrong.\n" << "See input format below.\n";
        printHelp();
        return 1;
    }
    inputf.clear();
    inputf.seekg(0);

    string a;
    int i = 0, j = 0;
    Matrix input;
    input.resize(rows, vector<float>(rows, 0.0));
    while (getline(inputf, a)) {
        if (i == rows) {
            i = 0;
            j++;
        }
        input[i++][j] = stof(a);
    }

    // Showing input
    cout << "Input Matrix: \n";
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < rows; j++) {
            cout << input[i][j] << "\t";
        }
        cout << "\n";
    }
    if (maxpooling) {
        cout << "Using maximum pooling.\n\n";
    } else {
        cout << "Using average pooling.\n\n";
    }

    // Processing
    float temp = 0;
    Matrix output;
    output.resize(size, vector<float>(size, 0.0));
    if (maxpooling) {
        for(int i = 0; i < size * stride; i+=stride) {
            for(int j = 0; j < size * stride; j+=stride) {
                for (int x = i; x < i + filter; x++) {
                    for (int y = j; y < j + filter; y++) {
                        temp = fmax(temp, input[x][y]);
                    }
                }
                output[i/stride][j/stride] = temp;
                temp = 0;
            }
        }
    } else {
        for(int i = 0; i < size * stride; i+=stride) {
            for(int j = 0; j < size * stride; j+=stride) {
                for (int x = i; x < i + filter; x++) {
                    for (int y = j; y < j + filter; y++) {
                        temp += input[x][y];
                    }
                }
                output[i/stride][j/stride] = temp/(size*size);
                temp = 0;
            }
        }
    }

    // Output
    cout << "Output: \n";
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            cout << output[i][j] << "\t";
        }
        cout << "\n";
    }
}
