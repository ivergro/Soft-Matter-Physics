#pragma once
#include <vector>
#include <iostream>
#include "filewriter.h"
#include "gillespie.h"
using namespace std;

class LV {
    //Variables
    double k1 = 3.;
    double k2 = 0.01;
    double k3 = 5.;
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
        //Setters
        void change_ks(const double new_k1, const double new_k2, const double new_k3);

        //Savers
        void save_vecs_IC(string timesteps, string IC_A1, string IC_A2);
        void save_vecs_ks(string timesteps, string IC_A1, string IC_A2);
    private:
        void calculate_A1_dt();
        void calculate_A2_dt();

};