#include <cmath>
#include <vector>

typedef std::vector<float> mat;

void NonLinearActivation(bool relu, mat& matrix) {
    int m = matrix.size();
    if (relu) {
        for (int i = 0; i < m; i++) {
            matrix[i] = fmax(matrix[i], 0);
        }
    } else {
        for (int i = 0; i < m; i++) {
            matrix[i] = tanh(matrix[i]);
        }
    }
}
