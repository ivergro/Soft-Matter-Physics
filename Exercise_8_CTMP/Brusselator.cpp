#include "Brusselator.h"

Brusselator::Brusselator(int x, int y, const double omega) : x(x), y(y), omega(omega) {
    t_vec.push_back(t);
    x_vec.push_back(x);
    y_vec.push_back(y);
}

//Public functions

void Brusselator::update_gillespie(){
    const double w1 = a * omega;
    const double w2 = x;
    const double w3 = 1/(omega*omega) * x*(x-1)*y;
    const double w4 = b * x;
    vector<double> ws = {w1, w2, w3, w4};
    const double lambda_C = w1 + w2 + w3 + w4;
    double update_time = draw_tau(lambda_C, 1); //Skal t taes inn?
    t += update_time;
    const int j = draw_state_j(ws, lambda_C);

    switch (j){
    case 0:
        x_birth();
        break;
    case 1:
        x_death();
        break;
    case 2:
        y_to_x();
        break;
    case 3:
        x_to_y();
        break;
    default:
        cout << "Invalid input" << endl;
        break;
    }

    t_vec.push_back(t);
    x_vec.push_back(x);
    y_vec.push_back(y);
}

void Brusselator::save_values_steps(string timesteps, string IC_x, string IC_y, string omega){
    string filepath = "./data/brusselator/steps/x=" + IC_x + "_y=" + IC_y + "_omega=" + omega + "/";
    classic_write_to_file(x_vec, filepath, "Xs_steps="+timesteps+".txt");
    classic_write_to_file(y_vec, filepath, "Ys_steps="+timesteps+".txt");
    classic_write_to_file(t_vec, filepath, "t_steps="+timesteps+".txt");
}

void Brusselator::save_values_tlimit(string t_limit, string IC_x, string IC_y, string omega){
    string filepath = "./data/brusselator/t_limit/x=" + IC_x + "_y=" + IC_y + "_omega=" + omega + "/";
    classic_write_to_file(x_vec, filepath, "Xs_tlimit="+t_limit+".txt");
    classic_write_to_file(y_vec, filepath, "Ys_tlimit="+t_limit+".txt");
    classic_write_to_file(t_vec, filepath, "t_tlimit="+t_limit+".txt");
}

 double Brusselator::get_t(){
    return t;
 }
//---------------------------------------------------------------------------------------------

//Private functions
void Brusselator::x_birth(){
    x++;
}

void Brusselator::x_death(){
    x--;
}

void Brusselator::y_to_x(){
    x++;
    y--;
}

void Brusselator::x_to_y(){
    x--;
    y++;
}