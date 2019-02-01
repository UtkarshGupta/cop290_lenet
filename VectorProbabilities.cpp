#include <cmath>
#include <vector>

using namespace std;

void VectorProbabilities(bool sigmoid, vector<float>& array, int a) {
    if (sigmoid) {
        for(int i = 0; i < a; i++) {
            array[i] = 1/(1+exp(-array[i]));
        }
    } else {
        float sum = 0;
        for(int i = 0; i < a; i++) {
            sum += exp(array[i]);
        }
        for(int i = 0; i < a; i++) {
            array[i] = exp(array[i])/sum;
        }
    }
}
