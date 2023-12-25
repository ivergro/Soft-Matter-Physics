#include "ising_model.h"
#include "utilities.h"

// Define global random number generators with different seeds
mt19937 metropolis_gen(1); // Seed for Metropolis function
mt19937 glauber_gen(2);    // Seed for Glauber function
mt19937 spin_gen(3);       // Seed for spin initialization

constexpr double K_b = 1;
constexpr double J   = 1;
const double T_c = 2.*J/(log(1+sqrt(2))*K_b); //Cant have constexpr when using func

bool metropolis(const double delta_E, const double beta){
    // random_device rd;
    uniform_real_distribution<double> distribution(0.0, 1.0); //Creates a uniform distribution from 0 to 1
    // double p = (double) rand() / RAND_MAX;
    double p = distribution(metropolis_gen);
    double a_ij = min(exp(-beta*delta_E), 1.);
    return p <= a_ij;
}

/**
 * @brief Performs on Monte Carlo sweep on the spin vector, changes it as it goes
 * 
 * @param L Dimension of 2D magnetic grid
 * @param T Temperature
 * @param spins vector with magnetic spins sigma = +-1. Taken in as a vector, changes happen "globally"
 * 
 * @return `int` New energy of matrix
 */
void glauber(const int L, const double T, int &E, int &M, vector<vector<int>> &spins){
    

    const double beta = 1/(K_b*T);
    uniform_int_distribution<int> distribution(0, L-1); 


    const int N = pow(L,2); //Number of spins
    for (int t = 0; t < N; t++){
        //Pick out one random position ij = [r_row][r_col] and returning the adress to that point
        int r_col = distribution(glauber_gen);
        int r_row = distribution(glauber_gen);
        int* sigma_pointer = &(spins.at(r_row)).at(r_col); //The spin pointer
        const int spin_val = *sigma_pointer;
        int delta_E = 2* spin_val * J *calculate_neighbour_values(L, spins, r_row, r_col); //Per def fra notes 4 (1.62)

        //Doing metropolis acceptance to see if the change in spin should happen. Ignoring it if not succeded.
        if (metropolis(delta_E, beta)){
            E += delta_E;
            M -= 2*spin_val;  //New total spin is previous minus 2 times the opposite spin value of chosen positions
            *sigma_pointer = -*sigma_pointer; //Changing spin
        }
    }
    // return pair<int,int> (E,E_squared);
}
/**
 * @brief Calculates eneryg given by function (1.58) in lecture notes 4, with nearest neighbours interacting and periodic BC.
 * @note
 * Positive J: Spin in same directions minimizes energy (Ferromagnetic).
 * Negative J: Spin in opposite directions minimizes energy (Anti Ferromagnetic).
 * @param spins 
 * @return int sum of total energy 
 */
int calculate_E(const int L, const vector<vector<int>> &spins){
    int sum = 0;
    for (int row = 0; row < L; row++) {
        for (int col = 0; col < L; col++) {
            int current_spin = (spins.at(row)).at(col);
            int row_neighbours = (spins.at(row)).at((L + col + 1) % L)*current_spin + (spins.at(row)).at((L + col - 1) % L)*current_spin; //Using modulo to ensure periodic BC
            int col_neighbours = (spins.at((L + row - 1) % L)).at(col)*current_spin + (spins.at((L + row + 1) % L)).at(col)*current_spin; //Using modulo to ensure periodic BC
            sum -= J*(row_neighbours + col_neighbours);
        }
    }
    return sum;
}

int calculate_neighbour_values(const int L, const vector<vector<int>> &spins,const int row,const int col){
    int row_neighbours = (spins.at(row)).at((L + col + 1) % L) + (spins.at(row)).at((L + col - 1) % L); //Using modulo to ensure periodic BC
    int col_neighbours = (spins.at((L + row - 1) % L)).at(col) + (spins.at((L + row + 1) % L)).at(col); //Using modulo to ensure periodic BC
    return row_neighbours + col_neighbours;
}

vector<vector<int>> initialize_spins(const int L, int &M){
    vector<vector<int>> spins;
    spins.reserve(L); //Reserving space for L vectors
    uniform_int_distribution<int> distribution(0,1); //Creates a uniform distribution from 0 to 1
    
    for (int row = 0; row < L; row++){
        spins.emplace_back(); //Adding a row
        spins.back().reserve(L); //Reserving space for L spins on newest row
        for (int col = 0; col < L; col++){
            int r_val = (distribution(spin_gen) % 2 == 0) ? -1 : 1; //Randomly picks from 0 to 1, if 0 -> -1, else 1
            (spins.back()).emplace_back(r_val);
            M += r_val;
        }
    }
    return spins;
}


int calculate_M(const vector<vector<int>> &spins){
    int sum = 0;
    for (const std::vector<int>& row : spins) {
        for (int value : row) {
            sum += value;
        }
    }
    return sum;
}


/**
 * @brief 2D Ising model for magnetic spin, ran at one constant temperature. Used to calibrate t_eq
 * 
 * @param T `double` Temperature to be used 
 * @param L `int` Number of magnets in each dimension. Min. 1
 * @param sweeps `int` Number of MC sweeps to be done. Mark: increases runtime drastically
 */
void run_IM(const double T, const int L, const int sweeps){
    //Constants
    int N = pow(L,2);
    int E = 0;
    int E_squared = 0;
    int save_every = (sweeps < 10000) ? 1 : sweeps / 10000;
    int M = 0;
    //Initialize
    vector<vector<int>> spins = initialize_spins(L, M);
    E = calculate_E(L, spins);
    vector<int> energies;
    vector<int> magnetizations;
    vector<vector<vector<int>>> saved_spin_values;
    cout << "Initial energy: " << E << endl;
    //MC sweeps
    energies.push_back(E);
    magnetizations.push_back(M);
    saved_spin_values.push_back(spins);
    for (int t = 1; t < sweeps; t++){
        glauber(L, T, E, M, spins); //Does the changes to the vector globally instead of returning the spins

        if (t%save_every == 0){ //Shows 5 sweep values
            //Saving values
            energies.push_back(E);
            magnetizations.push_back(calculate_M(spins));
        }
        if (t%100 == 0){
            saved_spin_values.push_back(spins); //Saving spin values
        }
    }

    //Saving values to be analyzed
    cout << "Final energy: " << E << "  after " << sweeps << " sweeps" << endl;
    string directory = "./data/T=" + to_string(T) + "/";
    classic_write_to_file(energies, directory, "energies_L="+to_string(L)+"_sweeps="+ to_string(sweeps)+".txt");
    classic_write_to_file(magnetizations, directory, "magnetizations_L="+to_string(L)+"_sweeps="+ to_string(sweeps)+".txt");
    // classic_write_3Dvec_to_file(saved_spin_values, directory, "saved_spin_values_L="+to_string(L)+"_sweeps="+ to_string(sweeps)+".txt");
}

void run_IM_different_temps(const double T, const int L, const int sweeps, const int T_num){
    //Constants
    int N = pow(L,2);
    int E = 0;
    int E_squared = 0;
    int save_every = (sweeps < 10000) ? 1 : sweeps / 10000;
    int M = 0;
    //Initialize
    vector<vector<int>> spins = initialize_spins(L, M);
    E = calculate_E(L, spins);
    vector<int> energies;
    vector<int> magnetizations;
    cout << "Initial energy: " << E << endl;
    //MC sweeps
    energies.push_back(E);
    magnetizations.push_back(M);
    for (int t = 1; t < sweeps; t++){
        glauber(L, T, E, M, spins); //Does the changes to the vector globally instead of returning the spins
        //Saving values
        energies.push_back(E);
        magnetizations.push_back(calculate_M(spins));
        
    }

    //Saving values to be analyzed
    cout << "Final energy: " << E << "  after " << sweeps << " sweeps" << endl;
    string directory = "./data/tempsweeps/L=" + to_string(L) + "/";
    classic_write_to_file(energies, directory, "energies_Tnum="+to_string(T_num)+"_sweeps="+ to_string(sweeps)+".txt");
    classic_write_to_file(magnetizations, directory, "magnetizations_Tnum="+to_string(T_num)+"_sweeps="+ to_string(sweeps)+".txt");
}



