#include "utilities.h"

/**
 * @brief Keeps particle position inside box
 * 
 * @param z position to be checked
 * @param L box size
 * @return double 
 */
double P_Img (double z, double L){
        
    double z1;
    z1 = z - floor(z/L)*L;
    return z1;
}

/**
 * @brief Calculates the minimum distance between two particles. 
 * 
 * @param dx distance between particles
 * @param L System size in 1 direction
 * @return double 
 */
double MinD(double dx, double L){

    double dx1;
    dx1 = dx - rint(dx/L)*L;
    return dx1;
}

