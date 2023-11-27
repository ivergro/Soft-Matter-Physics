#pragma once
#include "std_lib_facilities.h"
#include "filewriter.h"

//Exercise 2.2
double inversion_method(int n);

double F_inv(double x, int n);

double extra_inversion();


//Exercise 2.3
double random_r();

double random_theta();

pair<double, double> improved_circle_coord();

/**
 * Sample x and y values according to the the box-muller transformation, in a gaussian 2d distribution with mean=0 and var=1
 *
 * @param values Container whose values are summed.
 * @return x,y value 
 */
pair<double, double> Box_Muller_transformation();

//Excercise 2.3.1
double p_rej(const double x);

double g_rej(const double x, const double p);

double G_inv_rej(const double x, const double p);

vector<pair<double,double>> rejection_method(const int N, const double p);


//Exercise 2.4
double f_i(const double x);

double g(const double x);

double crude_method(const int N, const double a, const double b);

double Importanse_sampling_method(const int N, const double a, const double b);

//Ex cos

double g_cos(const double x, const double a, const double b);

double cos_integration(const int N, const double a, const double b);

void parameter_check();

//Variance and expectancy

pair<double,double> a_star(const double T);

double G_inv_var(double const a, double const rv);

int f_var(const double T, const double x);

double p_var(const double x);

double P_inv_var(const double rv);

double numerical_average(const double T, const int N);

double numerical_average_IS(const double T, const int N);

double varians(const double average_value);

double weird_varians(const double a, const double T);