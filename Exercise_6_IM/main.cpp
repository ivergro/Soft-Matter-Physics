#include "std_lib_facilities.h"
#include "ising_model.h"
#include "utilities.h"


//------------------------------------------------------------------------------'

int main() {
    cout << "1) Running Ising model with 3 temps with 3 different L values\n"
         << "2) Running IM for several temps, measuring X and magnetization\n"
         << endl;
    
    int input;
    cin >> input;

    switch(input){
        case 1:{
            const double Tc = 2.0 / log(1.0 + sqrt(2.0));
            const double T_low = 1.0; // low temperature
            const double T_high = 3.0; // high temperature
            const int sweeps = 1000;
            vector<int> L_values = {25, 50, 100};
            vector<double> Temps = {T_low, Tc, T_high};

            
            for (auto L : L_values){
                auto start_time = chrono::high_resolution_clock::now(); //Measuring time
                cout << "Running IM for L = " + to_string(L) << endl;
                for (auto T : Temps){
                    cout << "\tT = " + to_string(T) << endl;
                    run_IM(T, L, sweeps);
                    cout << "complete"<<endl;
                }
                auto end_time = chrono::high_resolution_clock::now();
                auto time = chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();
                cout << "Time taken for L=" + to_string(L) + " was: " << time << endl;
            } 
            break;
        } case 2:{
            vector<int> L_values = {10};
            const double Tc = 2.0 / log(1.0 + sqrt(2.0));
            const double T_low = 1.0; // low temperature
            const double T_high = 3.0; // high temperature
            const int sweeps = 1000;
            int M = 0;
            
            // for (auto l : L_values){
            //     run_IM(T_c, l, sweeps);
            // }

            const int L = 25;
            const vector<vector<int>> spins = initialize_spins(L, M);
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
            break;  
        }
    }
    return 0;
}

//------------------------------------------------------------------------------
