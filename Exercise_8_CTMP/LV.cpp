#include "LV.h"

LV::LV(int A1, int A2) : A1(A1), A2(A2){
    calculate_A1_dt();
    calculate_A2_dt();
    A1_vec.push_back(A1);
    A2_vec.push_back(A2);
    t_vec.push_back(t);
}

//Public functions
void LV::birth(){
    A1++;
}

void LV::kill_pluss_birth(){
    A1--;
    A2++;
}

void LV::pred_death(){
    A2--;
}

void LV::update_theoretical(double dt){
    calculate_A1_dt();
    calculate_A2_dt();
    A1 += A1_dt * dt;
    A2 += A2_dt * dt;
    A1_vec.push_back(A1);
    A2_vec.push_back(A2);
}

void LV::update_gillespie(){
    double lambda_C = k1 * A1 + k2 * A1 * A2 + k3 * A2;
    double update_time = draw_tau(lambda_C, 1); //Skal t taes inn?
    t += update_time;
    vector<double> w = {k1 * A1, k2 * A1 * A2, k3 * A2};
    const double j = draw_state_j(w, lambda_C);
    switch (int(j)){
        case 0:{
            birth();
            break;
        }case 1:{
            kill_pluss_birth();
            break;
        }case 2:{
            pred_death();
            break;
        }default:
            cout << "Invalid input" << endl;
            break;
    }
    A1_vec.push_back(A1);
    A2_vec.push_back(A2);
    t_vec.push_back(t);
}

//Getters
double LV::get_A1() const{
    return A1;
}
double LV::get_A2() const{
    return A2;
}

//Savers
void LV::save_vecs(string timesteps, string IC_A1, string IC_A2){
    string filepath = "./data/prey=" + IC_A1 + "_pred=" + IC_A2 + "/";
    classic_write_to_file(A1_vec, filepath, "A1_steps="+timesteps+".txt");
    classic_write_to_file(A2_vec, filepath, "A2_steps="+timesteps+".txt");
    classic_write_to_file(t_vec, filepath, "t_steps="+timesteps+".txt");
}


//--------------------

//Private functions
void LV::calculate_A1_dt(){
    A1_dt = k1 * A1 - k2 * A1 * A2;
}

void LV::calculate_A2_dt(){
    A2_dt = k2 * A1 * A2 - k3 * A2;
}

