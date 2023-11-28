#pragma once
#include "std_lib_facilities.h"
#include "filewriter.h"

//Constants
constexpr double K_b = 1;
constexpr double J   = 1;
const double T_c = 2.*J/(log(1+sqrt(2))*K_b); //Cant have constexpr when using func

bool metropolis(const double delta_E, const double T);

void glauber(const int L, const double T, int &E, int &M, vector<vector<int>> &spins);

int calculate_E(const int L, const vector<vector<int>> spins);

int calculate_neighbour_values(const int L, const vector<vector<int>> spins, const int row, const int col);

vector<vector<int>> initialize_spins(const int L);

int calculate_M(const vector<vector<int>> spins);

void run_IM(const double T, const int L, const int sweeps);

pair<double,double> run_IM_different_temps(const double T, const int L, const int sweeps,const vector<vector<int>> &spins);
