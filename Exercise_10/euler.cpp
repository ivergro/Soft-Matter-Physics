#include "euler.h"

double H(const double x, const double p, const double k = 1., const double m = 1.){
    return p*p/(2*m) + x*x*k/2;
}

double p_next(const double x_now, const double p_now, const double delta_t){
    return p_now - x_now*delta_t;
}

double first_x_next(const double x_now, const double p_now, const double delta_t){
    return x_now + p_now*delta_t;
}

double second_x_next(const double x_now, const double p_now, const double delta_t){
    return x_now + p_next(x_now, p_now, delta_t)*delta_t;
}

pair<double,double> first_euler_step(const double x_now, const double p_now, const double delta_t){
    const double x_next = first_x_next(x_now, p_now, delta_t);
    const double p_next_ = p_next(x_now, p_now, delta_t);
    return pair<double,double> (x_next, p_next_);
}

pair<double,double> second_euler_step(const double x_now, const double p_now, const double delta_t){
    const double x_next = second_x_next(x_now, p_now, delta_t);
    const double p_next_ = p_next(x_now, p_now, delta_t);
    return pair<double,double> (x_next, p_next_);
}

pair<double,double> analytical_step(const double x_now, const double p_now, const double delta_t, const double k = 1., const double m = 1.){
    const double x_next = x_now + p_now/m*delta_t;
    const double p_next_ = p_now -k*x_now*delta_t;
    return pair<double,double> (x_next, p_next_);
}

//Make one common euler? only one difference, which is the step, could take in int values 1 or 2 and run one of them
void first_euler(){
    const double T       = 10.;
    const double delta_t = 0.001;
    const int N          = (int) T/delta_t;
    vector<double> x_vec (N);
    vector<double> p_vec (N);

    //Initial conditions
    double x_now = 1;
    double p_now = 0;
    x_vec.at(0) = x_now;
    p_vec.at(0) = p_now;

    //Running Euler integration. i=0 is set from I.C.
    for (int i = 1; i < N; i++){
        pair<double,double> x_p_pair = first_euler_step(x_now, p_now, delta_t);
        x_vec.at(i) = x_p_pair.first;
        p_vec.at(i) = x_p_pair.second;
        x_now = x_p_pair.first;
        p_now = x_p_pair.second;
    }

    string filepath = "./data/ex_1/";
    string filename = "first_euler_dt=" + to_string(delta_t);

    write_to_file(x_vec, filepath, filename + "_x_vals");
    write_to_file(p_vec, filepath, filename + "_p_vals");
}


void second_euler(){
    const double T       = 10.;
    const double delta_t = 0.001;
    const int N          = (int) T/delta_t;
    vector<double> x_vec (N);
    vector<double> p_vec (N);

    //Initial conditions
    double x_now = 1;
    double p_now = 0;
    x_vec.at(0) = x_now;
    p_vec.at(0) = p_now;

    //Running Euler integration. i=0 is set from I.C.
    for (int i = 1; i < N; i++){
        pair<double,double> x_p_pair = second_euler_step(x_now, p_now, delta_t);
        x_vec.at(i) = x_p_pair.first;
        p_vec.at(i) = x_p_pair.second;
        x_now = x_p_pair.first;
        p_now = x_p_pair.second;
    }


    string filepath = "./data/ex_1/";
    string filename = "second_euler_dt=" + to_string(delta_t);

    write_to_file(x_vec, filepath, filename + "_x_vals");
    write_to_file(p_vec, filepath, filename + "_p_vals");
}

void analytical_harmonic_oscillator(){
    const double T       = 10.;
    const double delta_t = 0.001;
    const int N          = (int) T/delta_t;
    vector<double> x_vec (N);
    vector<double> p_vec (N);

    //Initial conditions
    double x_now = 1;
    double p_now = 0;
    x_vec.at(0) = x_now;
    p_vec.at(0) = p_now;

    //Running Euler integration. i=0 is set from I.C.
    for (int i = 1; i < N; i++){
        pair<double,double> x_p_pair = analytical_step(x_now, p_now, delta_t);
        x_vec.at(i) = x_p_pair.first;
        p_vec.at(i) = x_p_pair.second;
        x_now = x_p_pair.first;
        p_now = x_p_pair.second;
    }


    string filepath = "./data/ex_1/";
    string filename = "analytical_hc_dt=" + to_string(delta_t);

    write_to_file(x_vec, filepath, filename + "_x_vals");
    write_to_file(p_vec, filepath, filename + "_p_vals");
}