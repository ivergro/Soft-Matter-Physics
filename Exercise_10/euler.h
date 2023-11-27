#pragma once
#include "std_lib_facilities.h"
#include "filewriter.h"


double H(const double x, const double p, const double k , const double m );

double p_next(const double x_now, const double p_now, const double delta_t);

double first_x_next(const double x_now, const double p_now, const double delta_t);

double second_x_next(const double x_now, const double p_now, const double delta_t);

pair<double,double> first_euler_step(const double x_now, const double p_now, const double delta_t);

pair<double,double> second_euler_step(const double x_now, const double p_now, const double delta_t);

pair<double,double> analytical_step(const double x_now, const double p_now, const double delta_t, const double k, const double m );

//Make one common euler? only one difference, which is the step, could take in int values 1 or 2 and run one of them
void first_euler();


void second_euler();

void analytical_harmonic_oscillator();