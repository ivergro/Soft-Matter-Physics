#include "std_lib_facilities.h"
#include "ising_model.h"
#include "utilities.h"


//------------------------------------------------------------------------------'

int main() {
    vector<int> L_values = {10};
    const double Tc = 2.0 / std::log(1.0 + std::sqrt(2.0));
    const double T_low = 1.0; // Choose your low temperature
    const double T_high = 3.0; // Choose your high temperature
    const int sweeps = 1000;
    
    // for (auto l : L_values){
    //     run_IM(T_c, l, sweeps);
    // }

    const int L = 50;
    const vector<vector<int>> spins = initialize_spins(L);
    const vector<double> Temps = linspace(T_low,T_high,20);
    vector<double> specific_heat;
    vector<double> susceptibility;
    for(auto T : Temps){
        cout << "T: " << T << endl;
        pair<double,double> results = run_IM_different_temps(T, L, sweeps, spins);
        specific_heat.push_back(results.first);
        susceptibility.push_back(results.second);
    }
    cout << "Finished" << endl;
    string filepath = "./data/L="+to_string(L)+"/";
    classic_write_to_file(specific_heat, filepath, "specific_heat_sweeps="+to_string(sweeps)+".txt");
    classic_write_to_file(susceptibility, filepath, "susceptibility"+to_string(sweeps)+".txt");
    classic_write_to_file(Temps, filepath, "Temps");
    return 0;
}

//------------------------------------------------------------------------------
