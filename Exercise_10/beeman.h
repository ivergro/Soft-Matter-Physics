#pragma once
#include "std_lib_facilities.h"

vector<double> next_r_beeman(const vector<double>& r_now, const vector<double>& v_now, const vector<double>& f_now, const vector<double>& f_prev, const double dt, const double m=1.);

vector<double> next_v_beeman(const vector<double>& v_now, const vector<double>& f_next, const vector<double>& f_now, const vector<double>& f_prev, const double dt, const double m=1.);

vector<double> next_f_beeman(const vector<double>& r_next, const double k = 10.);

vector<double> v_dt(const vector<double>& r_0, const vector<double>& r_dt, const double dt);