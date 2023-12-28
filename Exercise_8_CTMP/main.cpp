#include "gillespie.h"
#include "filewriter.h"
#include "LV.h"
#include <iostream>
#include <vector>

using namespace std;

int main(){

    cout << "1) Lotka-Volterra model\n"
         << "2) Lotka-Volterra model with change in parameters\n"
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
        }
        default:
            cout << "Invalid input" << endl;
            break;
    }

    return 0;
}