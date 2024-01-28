#include "wolff.h"
#include <iostream>
#include <string>

using namespace std;

int main(){
    cout << "1) Cluster size statistics for one L\n"
         << "2) Observable calculations for three Ls\n"
         << "3) Multiple Markov Chains\n"
         << endl;
    int input;
    cin >> input;
    switch(input){
        case 1:{
            cout << "Cluster size statistics with\n";
            const int L = 100;
            const double T_c = 2/(log(1+sqrt(2)));
            const double T_h = 2*T_c;
            const double T_l = T_c/2;
            const int N = 10000;
            cout << "L = " << L << endl
                 << "N = " << N << endl
                 << "Ts: " << T_l << ", " << T_c << ", " << T_h << endl;
            for (auto T: {T_l, T_c, T_h}){
                IsingModel IM(L, T);
                string filepath = "./data/cluster_size_statistics/";
                string filename = filepath + "L="+to_string(L)+"_T="+to_string(T)+"_N="+to_string(N)+".csv";
                for (int i = 0; i < N; i++){
                    IM.step();
                    if (i%100 == 0 && i == (N-100)){
                        IM.save(filename, true);
                    } else{
                        IM.save(filename);
                    }
                } cout << "Saved to " << filename << endl;
                string filename_CS = filepath + "L="+to_string(L)+"_T="+to_string(T)+"_N="+to_string(N)+"_cluster_sizes.csv";
                IM.save_cluster_sizes(filename_CS);
                
            }
            break;
        } case 2:{
            cout << "Gathering observables\n";
            // const vector<int> Ls = {25,50,100};
            const vector<int> Ls = linspace_int(10,100,10);
            const double T_c = 2/(log(1+sqrt(2)));
            const int N = 10000;
            cout << "N = " << N << endl
                 << "T = " << T_c << endl;
            for (auto L: Ls){
                cout << "L = " << L << endl;
                IsingModel IM(L, T_c);
                for (int i = 0; i < N; i++){
                    IM.step();
                }
                string filepath_O = "./data/many_observables/";
                string filename_CS = filepath_O + "L="+to_string(L)+"_T="+to_string(T_c)+"_N="+to_string(N)+"_cluster_sizes.csv";
                string filename_E = filepath_O + "L="+to_string(L)+"_T="+to_string(T_c)+"_N="+to_string(N)+"_energies.csv";
                string filename_M = filepath_O + "L="+to_string(L)+"_T="+to_string(T_c)+"_N="+to_string(N)+"_magnetizations.csv";
                // IM.save_cluster_sizes(filename_CS);
                IM.save_energies(filename_E);
                IM.save_magnetizations(filename_M);
                cout << "Saved to " << filepath_O << endl;
            }
            break;
        } case 3:{
            //Sweeps, run swap test after each sweep, or after a number of sweeps
            //Constants
            cout << "Running MMC" << endl;
            const int L = 25;
            const int sweeps = 10000;
            const int swaps = sweeps;
            int num_swaps = 0; //Counting number of swaps
            const vector<double> temps = linspace(1., 5, 10);

            //Initializing spin pointer vector
            const int num_T_configs = temps.size();
            vector<IsingModel*> I_vec(num_T_configs);
            for (int i = 0; i < temps.size(); i++){
                I_vec.at(i) = new IsingModel(L,temps.at(i));
            }

            //Run model sweeps
            for(int j = 0; j < sweeps; j++){
                //One sweep per model
                for (int k = 0; k < num_T_configs; k++){
                    I_vec.at(k)->step();
                }
                //Running swap between temp configs for different spin configs
                if (j % (sweeps/swaps) == 0){
                    //TODO: Run swap func
                    swap_temp_configurations(I_vec, num_swaps);
                }
            }

            //End results
            cout << "Final energies:" << endl;
            for (int i = 0; i < temps.size(); i++){
                cout << i << ") E=" << I_vec.at(i)->get_E() << "\t T=" << I_vec.at(i)->get_T() <<endl;
            }

            //Results
            const double swap_percentage = swaps == 0 ? 0 : (double) num_swaps/(swaps * num_T_configs);
            const double total_swap_percentage = swaps == 0 ? 0 : (double) num_swaps/(sweeps * num_T_configs);
            cout << "Swap percentage: " << swap_percentage << " , ran with " << swaps << " number of swap tests" << endl;
            cout << "Swap percentage per step/sweeps: " << total_swap_percentage << endl;

            //Save magnetizations to autocorrelation calculations
            string filepath = "./data/MMC/";
            if (swaps == 0){
                filepath = "./data/MMC_no_swap/";
            }

            //Saving swapping percentages
            std::ofstream outfile(filepath + "percentages_L="+to_string(L)+"_N="+to_string(sweeps)+".txt");
            outfile << "Total swap percentage: " << total_swap_percentage << "\n";
            outfile.close();

            for (int i = 0; i < temps.size(); i++){
                string filename_M = filepath + "L="+to_string(L)+"_T="+to_string(temps[i])+"_N="+to_string(sweeps)+"_magnetizations.csv";
                I_vec.at(i)->save_magnetizations(filename_M);
            }
            cout << "Saved to path: " << filepath << endl;
            break;
        }
    }

    return 0;
}
