#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int VectorProbabilities(char** argv) {
    string str = argv[1];
    bool sigmoid = str.find("sigmoid") != string::npos;
    ifstream file(argv[2]);
    int size = atoi(argv[3]);

    string a;
    vector<float> array;
    while (getline(file, a)) {
        array.push_back(stof(a));
    }

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

    for(int i = 0; i < size; i++) {
        cout << array[i] << "\n";
    }

    return 0;
}
