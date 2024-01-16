#pragma once
#include "std_lib_facilities.h"

double next_x(const double x_now, const double v_now, const double f_now, const double dt, const double m=1.);

double next_v(const double v_now, const double f_now, const double f_next, const double dt, const double m=1.);

double next_f(const double x_now, const double x_eq = 0., const double k=10.);