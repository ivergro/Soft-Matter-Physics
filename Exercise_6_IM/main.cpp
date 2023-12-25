#include "ising_model.h"
#include "utilities.h"
using namespace std;

//------------------------------------------------------------------------------'
int main() {
    cout << "1) Running Ising model with 3 temps with 3 different L values\n"
         << "2) Running IM for several temps, measuring X and magnetization\n"
         << "3) Running IM for several Ls, T=Tc\n"
         << endl;
    
    int input;
    cin >> input;

    switch(input){
        case 1:{
            const double Tc = 2.0 / log(1.0 + sqrt(2.0));
            const double T_low = 1.0; // low temperature
            const double T_high = 3.0; // high temperature
            const int sweeps = 1000;
            vector<int> L_values = {25};//, 50, 100};
            vector<double> Temps = {2.5};

            
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
            const double Tc = 2.0 / log(1.0 + sqrt(2.0));
            const double T_low = 1.0; // low temperature
            const double T_high = 3.0; // high temperature
            
            const int sweeps = 10000;
            vector<int> L_values = {100};
            vector<double> Temps = linspace(0.1,5,20);

            
            for (auto L : L_values){
                auto start_time = chrono::high_resolution_clock::now(); //Measuring time
                cout << "Running IM for L = " + to_string(L) << endl;
                int T_num = 1;
                for (auto T : Temps){
                    cout << "\tT = " + to_string(T) << endl;
                    run_IM_different_temps(T, L, sweeps, T_num++);
                    cout << "complete"<<endl;
                }
                auto end_time = chrono::high_resolution_clock::now();
                auto time = chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();
                cout << "Time taken for L=" + to_string(L) + " was: " << time << endl;
            }
            break;  
        } case 3:{
            const double Tc = 2.0 / log(1.0 + sqrt(2.0));
            vector<int> L_values = {10,20,30,40,50,60,70,80,90,100};
            const int sweeps = 10000;
            for (auto L : L_values){
                auto start_time = chrono::high_resolution_clock::now(); //Measuring time
                cout << "Running IM for L = " + to_string(L) << endl;
                run_IM(Tc, L, sweeps);
                auto end_time = chrono::high_resolution_clock::now();
                auto time = chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();
                cout << "Time taken for L=" + to_string(L) + " was: " << time << endl;
            }
            break;
        }
    }
    return 0;
}

//------------------------------------------------------------------------------
