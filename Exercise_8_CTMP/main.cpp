#include "gillespie.h"
#include "filewriter.h"
#include "LV.h"
#include <iostream>
#include <vector>

using namespace std;

int main(){

    cout << "1) Lotka-Volterra model\n"
         << endl;

    int input;
    cin >> input;

    switch(input){
        case 1:{
            cout << "Lotka-Volterra model" << endl;

            //Initial conditions
            int IC_A1 = 500; //Prey
            int IC_A2 = 280; //Predator
            int steps = 100000;
            
            LV LV_model = LV(IC_A1, IC_A2);
            for (int t = 0; t < steps; t++){
                LV_model.update_gillespie();
            }
            LV_model.save_vecs(to_string(steps), to_string(IC_A1), to_string(IC_A2));
            break;
        }default:
            cout << "Invalid input" << endl;
            break;
    }

    return 0;
}