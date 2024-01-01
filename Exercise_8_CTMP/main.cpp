#include "gillespie.h"
#include "filewriter.h"
#include "LV.h"
#include "Brusselator.h"
#include <iostream>
#include <vector>

using namespace std;

int main(){

    cout << "1) Lotka-Volterra model\n"
         << "2) Lotka-Volterra model with change in parameters\n"
         << "3) Brusselator steps\n"
         << "4) Brusselator until t=48\n"
         << endl;

    int input;
    cin >> input;

    switch(input){
        case 1:{
            cout << "Lotka-Volterra model" << endl;


            //Initial conditions
            int IC_A1 = 100; //Prey
            int IC_A2 = 50; //Predator
            int steps = 100000;
            
            LV LV_model = LV(IC_A1, IC_A2);
            for (int t = 0; t < steps; t++){
                LV_model.update_gillespie();
            }
            LV_model.save_vecs_IC(to_string(steps), to_string(IC_A1), to_string(IC_A2));
            break;
        } case 2:{
            cout << "Lotka-Volterra model with change in parameters" << endl;

            vector<vector<double>> all_ks {{3., 0.1, 5.}, {3., 0.001, 5.}, {5., 0.01, 5.}, {1., 0.01, 5.}, {3., 0.01, 1.}, {3., 0.01, 10.}};

            int IC_A1 = 500; //Prey
            int IC_A2 = 280; //Predator
            int steps = 100000;
            for (auto ks : all_ks){
                LV LV_model = LV(IC_A1, IC_A2);
                LV_model.change_ks(ks[0], ks[1], ks[2]);
                for (int t = 0; t < steps; t++){
                    LV_model.update_gillespie();
                }
                LV_model.save_vecs_ks(to_string(steps), to_string(IC_A1), to_string(IC_A2));
            }
            break;
        } case 3:{
            cout << "Brusselator running" << endl;
            int IC_x = 2200;
            int IC_y = 2200;
            const int omega = 10000;
            const int steps = 100000;
            Brusselator bruss_model = Brusselator(IC_x, IC_y, omega);
            for (int t = 0; t < steps; t++){
                bruss_model.update_gillespie();
            }
            bruss_model.save_values_steps(to_string(steps), to_string(IC_x), to_string(IC_y), to_string(omega));

            break;
        } case 4:{
            cout << "Brusselator running until 48" << endl;
            vector<int> vec_IC_x {2200, 100, 0};
            vector<int> vec_IC_y {100, 2200, 0};
            vector<int> vec_omegas {100, 1000, 10000};
            const double t_limit = 48.;
            for (auto omega : vec_omegas){
                for (int i = 0; i < vec_IC_x.size(); i++){
                    int IC_x = vec_IC_x[i];
                    int IC_y = vec_IC_y[i];
                    Brusselator bruss_model = Brusselator(IC_x, IC_y, omega);
                    double t = bruss_model.get_t();
                    while (t < t_limit){
                        bruss_model.update_gillespie();
                        t = bruss_model.get_t();
                    }
                    bruss_model.save_values_tlimit(to_string(t_limit), to_string(IC_x), to_string(IC_y), to_string(omega));
                }
            }
        }
        default:
            cout << "Invalid input" << endl;
            break;
    }

    return 0;
}