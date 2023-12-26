#pragma once
#include <vector>
#include <iostream>
#include "filewriter.h"
#include "gillespie.h"
using namespace std;

class LV {
    //Variables
    const double k1 = 3.;
    const double k2 = 0.01;
    const double k3 = 5.;
    double t = 0;
    int A1;//Prey
    int A2;//Predator
    double A1_dt;
    double A2_dt;
    vector<int> A1_vec;
    vector<int> A2_vec;
    vector<double> t_vec;

    public:
        LV(int A1, int A2);
        void birth();
        void kill_pluss_birth();
        void pred_death();
        void update_theoretical(double dt);
        void update_gillespie();
        //Getters
        double get_A1() const;
        double get_A2() const;

        //Savers
        void save_vecs(string timesteps, string IC_A1, string IC_A2);
    private:
        void calculate_A1_dt();
        void calculate_A2_dt();

};