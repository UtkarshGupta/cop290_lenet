#include <cmath>
#include <vector>

typedef std::vector<float> mat;

void VectorProbabilities(bool softmax, mat& array) {
    int a = array.size();
    if (softmax) {
        float t = 0;
        for (int i = 0; i < a; i++) {
            t += exp(array[i]);
        }
        for (int i = 0; i < a; i++) {
            array[i] = exp(array[i])/t;
        }
    } else {
        for (int i = 0; i < a; i++) {
            array[i] = 1/(1+exp(-array[i]));
        }
    }
}
