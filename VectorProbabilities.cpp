#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

typedef vector<float> Array;

void printHelp() {
    ifstream in("./README.md");
    if (!in) {
        cout << "README.md missing.\n";
        return;
    }

    string a;
    for (int i = 0; i <= 48; i++) {
        getline(in, a);
    }
    for (int i = 49; i <= 60; i++) {
        getline(in, a);
        cout << a << endl;
    }
}

int main(int argc, char** argv) {
    bool sigmoid;
    int size;
    string file;

    // Arguments processing
    if (argc != 4) {
        cout << "Incorrect number Arguments, see format below.\n";
        printHelp();
        return 1;
    }

    string str = argv[1];
    if (str.find("sigmoid") != string::npos) {
        sigmoid = true;
    } else if (str.find("softmax") != string::npos) {
        sigmoid = false;
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
        size = atoi(argv[3]);
    } else {
        cout << "Error in Argument 3, see format below.\n";
        printHelp();
        return 1;
    }

    // Input processing
    ifstream arrayf(file);
    if (!arrayf) {
        cout << "Cannot open input file: " << file << ".\nSee input format below.\n";
        printHelp();
        return 1;
    } else if (count(istreambuf_iterator<char>(arrayf), istreambuf_iterator<char>(), '\n') != size) {
        cout << "Input file length wrong.\n" << "See input format below.\n";
        printHelp();
        return 1;
    }
    arrayf.clear();
    arrayf.seekg(0);

    string a;
    Array array;
    while (getline(arrayf, a)) {
        array.push_back(stof(a));
    }

    // Showing input
    cout << "Input Matrix: \n";
    for(int i = 0; i < size; i++) {
        cout << array[i] << "\n";
    }
    if (sigmoid) {
        cout << "Using sigmoid.\n\n";
    } else {
        cout << "Using softmax.\n\n";
    }

    // Processing
    if (sigmoid) {
        for(int i = 0; i < size; i++) {
            array[i] = 1/(1+exp(-1 * array[i]));
        }
    } else {
        int sum = 0;
        for(int i = 0; i < size; i++) {
            sum += exp(array[i]);
        }
        for(int i = 0; i < size; i++) {
            array[i] = exp(array[i])/sum;
        }
    }

    // Output
    cout << "Output: \n";
    for(int i = 0; i < size; i++) {
        cout << array[i] << "\n";
    }
}
