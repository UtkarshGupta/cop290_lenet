#include <cmath>
#include <vector>

using namespace std;

void NonLinearActivation(bool relu, vector<float>& matrix, int r, int c) {
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
}
