#pragma once
#include <vector>
#include <random>
#include <iostream>
using namespace std;
//Function to draw at random, the "tau", next reaction to occur

//går fra x-> inf to inf
double draw_tau(const double lambda_C, const double t);


//Function to choose next state "j" to transition to from "C"

int draw_state_j(const vector<double>& w, const double lambda_C);