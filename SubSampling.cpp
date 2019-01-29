#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int SubSampling(char** argv) {
    string str = argv[1];
    bool maxpooling = str.find("maxpooling") != string::npos;
    ifstream file(argv[2]);
    int size = atoi(argv[3]);
    int filter = atoi(argv[4]);
    int stride = atoi(argv[5]);
    int output_size = (size - filter)/stride + 1;

    string a;
    int t = 0;
    vector<float> input;
    input.reserve(size*size);
    while (getline(file, a)) {
        if (t >= size*size) {
            t -= size*size - 1;
        }
        input[t] = stof(a);
        t += size;
    }

    float temp = 0;
    vector<float> output;
    output.reserve(output_size*output_size);
    if (maxpooling) {
        for(int i = 0; i < output_size * stride; i+=stride) {
            for(int j = 0; j < output_size * stride; j+=stride) {
                for (int x = i; x < i + filter; x++) {
                    for (int y = j; y < j + filter; y++) {
                        temp = fmax(temp, input[x*size + y]);
                    }
                }
                output[i/stride*output_size + j/stride] = temp;
                temp = 0;
            }
        }
    } else {
        for(int i = 0; i < output_size * stride; i+=stride) {
            for(int j = 0; j < output_size * stride; j+=stride) {
                for (int x = i; x < i + filter; x++) {
                    for (int y = j; y < j + filter; y++) {
                        temp += input[x*size + y];
                    }
                }
                output[i/stride*output_size + j/stride] = temp/(filter*filter);
                temp = 0;
            }
        }
    }

    for(int i = 0; i < output_size; i++) {
        for(int j = 0; j < output_size; j++) {
            cout << output[i*output_size + j] << "\t";
        }
        cout << "\n";
    }

    return 0;
}
