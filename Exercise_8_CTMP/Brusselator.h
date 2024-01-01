#pragma once
#include "gillespie.h"
#include "filewriter.h"
#include <vector>
#include <random>
#include <iostream>
using namespace std;

class Brusselator{
//Variables
    const double a = 2;
    const double b = 5;
    const double omega;
    double t = 0;
    int x;
    int y;
    vector<double> t_vec;
    vector<int> x_vec;
    vector<int> y_vec;

public:
    Brusselator(int x, int y, const double omega);
    void update_gillespie();
    void save_values_steps(string timesteps, string IC_x, string IC_y, string omega);
    void save_values_tlimit(string t_limit, string IC_x, string IC_y, string omega);
    double get_t();
private:
    void x_birth();
    void x_death();
    void y_to_x();
    void x_to_y();
};