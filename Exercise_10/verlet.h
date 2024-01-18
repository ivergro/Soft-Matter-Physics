#pragma once
#include "std_lib_facilities.h"

vector<double> next_r(const vector<double>& r_now, const vector<double>& r_prev, const vector<double>& f_now,const double dt, const double m = 1.);

//Misleading name, calculates forces for current position
vector<double> next_f_verlet(const vector<double>& r_now, const double k = 10.);

//Notice r_next-r_prev, not r_now
vector<double> v_now_verlet(const vector<double>& r_next, const vector<double>& r_prev,const double dt);

//Used to calculate first r movement since we cannot use standard I.C.
vector<double> r_dt(const vector<double>& r_0, const vector<double>& v_0, double dt,const double m = 1.);

vector <double> r_half_kick(const vector<double>& r_now, const vector<double>& v_now, const vector<double>& f_now, const double dt, const double m = 1.);