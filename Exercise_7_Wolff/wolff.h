#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <fstream>

using namespace std;


class IsingModel {
    int L;
    double T;
    double E;
    int M;
    vector<vector<int>> spins;
    vector<vector<bool>> cluster;
    vector<int> cluster_sizes;
    vector<int> magnetizations;
    vector<int> energies;
    mt19937 gen;
    uniform_real_distribution<> dis;
    int stepCount = 0;
    int current_cluster_size = 0;
public:
    IsingModel(int L, double T);

    void step(); //Or sweep

    //Saving funcs
    void save(const string& filename, bool final_save = false);
    void save_cluster_sizes(const string& filename);
    void save_magnetizations(const string& filename);
    void save_energies(const string& filename);
    
    //Getters and setters
    const double get_T() const;
    void set_T(const double T);
    const double get_E() const;

private:
    void growCluster(int i, int j, int spin);

    int calculate_E();
    int calculate_M();

};

//Help funcs
vector<double> linspace(double start, double end, int numPoints);
vector<int> linspace_int(int start, int end, int numPoints);

//MMC funcs
bool swap_condition(const IsingModel* low_temp_model, const IsingModel* high_temp_model);
void update_positions(vector<IsingModel*> &I_vec, const int k);
void swap_temp_configurations(vector<IsingModel*> &I_vec, int &num_swaps);
