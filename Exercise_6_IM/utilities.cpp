#include "utilities.h"

/**
 * @brief Created a linspace function similar to np.linspace
 * 
 * @param start 
 * @param end 
 * @param numPoints 
 * @return vector<double> 
 */
vector<double> linspace(double start, double end, int numPoints) {
    vector<double> result;
    if (numPoints <= 1) {
        result.push_back(start);
        return result;
    }
    
    double step = (end - start) / (numPoints - 1);
    for (int i = 0; i < numPoints; ++i) {
        result.push_back(start + i * step);
    }
    return result;
}