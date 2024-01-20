#pragma once
#include "std_lib_facilities.h"
// #include "deifinitions.h"

/**
 * @brief Keeps particle position inside box
 * 
 * @param z position to be checked
 * @param L box size
 * @return double 
 */
double P_Img (double z, double L);

/**
 * @brief Calculates the minimum distance between two particles. 
 * 
 * @param dx distance between particles
 * @param L System size in 1 direction
 * @return double 
 */
double MinD(double dx, double L);


//Has to be declared here, otherwise it won't compile
template <typename T>
string vec_str(vector<T> vec){
    string str = "[";
    for (int i = 0; i < vec.size(); i++){
        str += to_string(vec[i]);
        if (i < vec.size()-1){
            str += ", ";
        }
    }
    str += "]";
    return str;
}

template <typename T>
stringstream vecofvec_to_ss(const vector<vector<T>>& vec){
    stringstream ss;
    for (const auto& inner_vec : vec) {
        for (const auto& element : inner_vec) {
            ss << element << " ";
        }
        ss << "\n"; // Adding new line
    }
    return ss;
}