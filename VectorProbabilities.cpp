#include <algorithm>
#include <cmath>
#include <vector>

typedef std::vector<float> mat;

void VectorProbabilities(bool softmax, mat& array) {
    int a = array.size();
    if (softmax) {
        float max = *std::max_element(array.begin(), array.end());
        float sum = 0;
        for (int i = 0; i < a; i++) {
            sum += exp(array[i] - max);
        }
        for (int i = 0; i < a; i++) {
            array[i] = exp(array[i] - max)/sum;
        }
    } else {
        for (int i = 0; i < a; i++) {
            array[i] = 1/(1+exp(-array[i]));
        }
    }
}
