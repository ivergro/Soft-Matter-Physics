#include "std_lib_facilities.h"
#include "filewriter.h"
#include "hit_or_miss.h"
#include "sampling.h"
#include "markov_chain.h"
#include "matrix.h"



//Forbedringer

int main(){
    unsigned int seed = 100;
    srand(seed);
    cout << "Menu\n"
         << "1) Ex 2.1 hit or miss\n"
         << "2) Ex 2.2 Inversion method\n"
         << "3) Ex 2.3 Transformation of coordinates\n"
         << "4) Ex 2.3.1 Rejection method\n"
         << "5) Ex 2.4 Importance Sampling\n"
         << "6) Ex 2.4 Numerical average\n"
         << "7) Ex 2.5 Markov chains\n"
         << "8) Ex 2.6 Additional exercises\n"
         << "Default exit\n"
         << "input: ";
    int input = -1;
    cin >> input;
    cout << endl;

    switch(input){
        case 1:{
            vector<int> N_values {10,100,1000,10000,100000};
            vector<double> num_results (N_values.size());

            for (int i = 0; i < N_values.size(); i++){
                double num_vs_actual_rectangle = hit_or_miss_rectangle(N_values.at(i));
                double num_vs_actual_disk = hit_or_miss_disk(N_values.at(i));
                
                cout << "N: " << N_values.at(i) <<"\n\t";
                cout << "Numerical/actual area calculated of rectangel: " << num_vs_actual_rectangle << "\n\t"
                     << "Numerical/actual area calculated of disk: " << num_vs_actual_disk << "\n";
            }
            break;
        } case 2:{
            int N = 100000;
            int n = 3;
            cout << "Inversion method:\n";
            vector<double> random_numbers(N);
            vector<double> extra_RN(N);
            for (int i=0; i < N; i++){
                double temp = inversion_method(n);
                double extra_temp = extra_inversion();
                // cout << i << ": " << temp << endl;
                random_numbers.at(i) = temp;
                extra_RN.at(i) = extra_temp;

            }
            classic_write_to_file(random_numbers, "./data/ex_2_2/","n=" + to_string(n) + "_N=" + to_string(N) + ".txt");
            // classic_write_to_file(extra_RN, "./data/ex_2_2/", "X_from_0_to_2_N=" + to_string(N) + ".txt");
            break;
        } case 3:{
            //A bit big, could split into several cases
            cout << "Unit disk sampling:\n";
            const int N = 10000;
            const double mean = 2;
            const double var = 5;
            //(r,theta)
            vector<pair<double,double>> random_pairs(N);
            vector<pair<double,double>> improved_random_pairs(N);
            vector<pair<double,double>> Box_Muller_pairs(N);
            vector<pair<double,double>> Box_Muller_pairs_shifted(N);
            for (int i = 0; i < N; i++){
                random_pairs.at(i) = pair<double,double> (random_r(), random_theta());
                improved_random_pairs.at(i) = improved_circle_coord();
                pair<double,double> x_y = Box_Muller_transformation();
                Box_Muller_pairs.at(i) = x_y;
                Box_Muller_pairs_shifted.at(i) = pair<double,double> (mean + sqrt(var)*x_y.first, mean + sqrt(var)*x_y.second);
            }
            string filepath = "./data/ex_2_3/";
            // classic_write_pair_to_file(random_pairs, filepath, "unit_disk_N=" + to_string(N) + ".txt");
            // classic_write_pair_to_file(improved_random_pairs, filepath, "improved_unit_disk_N=" + to_string(N) + ".txt");
            // classic_write_pair_to_file(Box_Muller_pairs, filepath, "Box_Muller_N=" + to_string(N) + ".txt");
            // classic_write_pair_to_file(Box_Muller_pairs_shifted, filepath, "Box_Muller_shifted_N=" + to_string(N) + ".txt");
            break;
        } case 4:{
            const int N = 10000;
            const vector<double> ps {0.4,0.6};
            cout << "Rejection Method: " << endl;
            for (auto p : ps){
                cout << "p: " << p << endl
                     << "A: " << 1/(p+1/(2*p)) << endl;
                vector<pair<double,double>> points = rejection_method(N, p);
                string filepath = "./data/ex_2_3_1/";
                string filename = "rej_meth_N=" + to_string(N) + "_p=" + to_string(p) + ".txt";
                classic_write_pair_to_file(points, filepath, filename);
            }
            break;
        } case 5:{
            const int N = 100000;
            const double a = 0.;
            const double b = 100.; //To large b results in bad importance sampling. I think x values may fuck them a bit up
            cout << "parameters:"  << endl
                 << "\t N = " << N << endl
                 << "\t a = " << a << endl
                 << "\t b = " << b << endl;
            cout << "Crude Method for e^(-x^2) with slowly varying func g(x)" << endl;
            const double analytical_sol = sqrt(M_PI)/2;
            const double numerical_sol_crude = crude_method(N, a, b);
            cout << "Comparing results:\n" 
                 << "Analytical area. Not correct atm: " << analytical_sol << endl
                 << "Numerical area: " << numerical_sol_crude << endl;

            cout << "\nImportance sampling method for e^(-x^2) with slowly varying func g(x)" << endl;
            const double numerical_sol_import = Importanse_sampling_method(N, a, b);
            cout << "Comparing results:\n" 
                 << "Analytical area: " << analytical_sol << endl
                 << "Numerical area: " << numerical_sol_import << endl;

            // cout << "Cos(x) integration, parameter check" << endl;
            // parameter_check();
            break;
        } case 6:{
            cout << "Numerical average chosen\n";
            const vector<double> Ts {3.,5.,10.,20.};
            const int N = 1000;
            for (auto T : Ts){
                cout << "T = " << T << endl;
                cout << "N = " << N << endl;
                const double num_av = numerical_average(T, N);
                const double num_av_IS = numerical_average_IS(T, N);
                const double an_av  = exp(-T);
                const double a_str  = a_star(T).second;
                cout << "Numerical average = " << num_av << endl
                     << "Numerical average improved = " << num_av_IS << endl
                     << "Analytical average = " << an_av << endl
                     << "Numerical var = " << varians(num_av) << endl
                     << "Numerical weird var = " << weird_varians(a_str, T) << endl
                     << "Analytical var = " << varians(an_av) << endl;
                
                cout << endl
                     << "Calculations:" << endl
                     << "an_sigma^2/an_average = " << varians(an_av)/an_av << endl
                     << "sigma_weird^2/an_average" << weird_varians(a_str, T)/an_av << endl
                     << "sigma^2/sigma_weird^2 = " << varians(an_av)/weird_varians(a_str, T) << endl;

                cout << endl; 
            } 
            break;
        } case 7:{
            cout << "Period of P_1\n";
            Matrix current_vec(2,1);
            current_vec.set(0,0,1);
            find_periodicity(P_1(), current_vec, 1000);

            const int N = 100;
            cout << "P^N of P_1 and P_2" << endl;
            calculate_P_n(P_1(), N);
            calculate_P_n(P_2(), N);

            cout << "\nChecking if P_15 is regular:\n";
            calculate_P_n(P_15(), N);

            cout << "\nChecking if P_16 is regular:\n";
            calculate_P_n(P_16(), N);

            cout << "\nFinding pi, the stationairy distribution, of P_18:\n";
            Matrix start_vec(3,1);
            start_vec.set(1,0,1.);
            find_periodicity(P_18(), start_vec, 10000);
            break;
        } case 8:{
            int N = 100000;
            double mu = 10;
            double a = 2;
            double n = 2;
            cout << "Inversion method of additional exercises:\n";
            vector<double> random_numbers_1(N);
            vector<double> random_numbers_2(N);
            vector<double> random_numbers_3(N);
            // vector<double> extra_RN(N);
            for (int i=0; i < N; i++){
                double rv = (double) rand() / RAND_MAX;
                random_numbers_1.at(i) = add_inv_1(rv, mu);
                random_numbers_2.at(i) = add_inv_2(rv);
                random_numbers_3.at(i) = add_inv_3(rv, a, n);

            }
            classic_write_to_file(random_numbers_1, "./data/add_inv/","1_mu=" + to_string(mu) + "_N=" + to_string(N) + ".txt");
            classic_write_to_file(random_numbers_2, "./data/add_inv/","2_N=" + to_string(N) + ".txt");
            classic_write_to_file(random_numbers_3, "./data/add_inv/","3_a=" + to_string(a) + "_n=" + to_string(n) + "_N=" + to_string(N) + ".txt");
        }
        default:
            break;
    }
    
    // classic_write_to_file(num_results, "./test_file.txt");
    // vector<double> myVec = classic_load_from_file<double>("./test_file.txt");
    // cout << myVec.at(2);
    return 0;
}




