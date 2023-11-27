#pragma once
#include "std_lib_facilities.h"

//Could use both a or a=f/m
double update_r(const double r_now, const double v_now, const double a_now, const double dt);

double half_kick(const double v_now, const double a_now, const double a_next, const double dt);