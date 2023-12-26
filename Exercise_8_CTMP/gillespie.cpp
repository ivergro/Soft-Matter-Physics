#include "gillespie.h"

//går fra x-> inf to inf
double draw_tau(const double lambda_C, const double t){
    random_device rd;
    mt19937 gen(rd());
    exponential_distribution<> d(lambda_C * t);
    double tau = d(gen);
    return tau;
}


//Function to choose next state "j" to transition to from "C"

int draw_state_j(const vector<double>& w, const double lambda_C){
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0, 1);
    double r = dis(gen);
    double sum = 0;
    for (int i = 0; i < w.size(); i++){
        sum += w[i]/lambda_C;
        if (sum >= r){
            return i;
        }
    }
    cout << "None of the states were chosen" << endl;
    return 0;
}