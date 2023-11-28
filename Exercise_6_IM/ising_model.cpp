#include "ising_model.h"
#include "utilities.h"

// Define global random number generators with different seeds
mt19937 metropolis_gen(1); // Seed for Metropolis function
mt19937 glauber_gen(2);    // Seed for Glauber function
mt19937 spin_gen(3);       // Seed for spin initialization

bool metropolis(const double delta_E, const double T){
    // random_device rd;
    uniform_real_distribution<double> distribution(0.0, 1.0); //Creates a uniform distribution from 0 to 1
    // double p = (double) rand() / RAND_MAX;
    double p = distribution(metropolis_gen);
    double beta = 1/(K_b*T);
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
    //Random numbers from 0 to L-1
    // int r_col = rand() % L; 
    // int r_row = rand() % L;
    mt19937 gen(1); //seed
    uniform_int_distribution<int> distribution(0, L-1); 
    int r_col = distribution(glauber_gen);
    int r_row = distribution(glauber_gen);
    const int N = pow(L,2); //Number of spins
    for (int t = 0; t < N; t++){
        //Pick out one random position ij = [r_row][r_col] and returning the adress to that point
        int* sigma_pointer = &(spins.at(r_row)).at(r_col); //The spin pointer
        const int spin_val = *sigma_pointer;
        int delta_E = 2*spin_val * J *calculate_neighbour_values(L, spins, r_row, r_col); //Per def fra notes 4 (1.62)

        //Doing metropolis acceptance to see if the change in spin should happen. Ignoring it if not succeded.
        if (metropolis(delta_E, T)){
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
int calculate_E(const int L, const vector<vector<int>> spins){
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

int calculate_neighbour_values(const int L, const vector<vector<int>> spins,const int row,const int col){
    int row_neighbours = (spins.at(row)).at((L + col + 1) % L) + (spins.at(row)).at((L + col - 1) % L); //Using modulo to ensure periodic BC
    int col_neighbours = (spins.at((L + row - 1) % L)).at(col) + (spins.at((L + row + 1) % L)).at(col); //Using modulo to ensure periodic BC
    return row_neighbours + col_neighbours;
}

vector<vector<int>> initialize_spins(const int L){
    vector<vector<int>> spins(L,vector<int>(L));
    uniform_int_distribution<int> distribution(0,1); //Creates a uniform distribution from 0 to 1
    
    for (int row = 0; row < L; row++){
        vector<int> *current_row_pointer = &spins.at(row); //Using pointer to try and save run time by extracting each row once
        for (int col = 0; col < L; col++){
            int r_val = (distribution(spin_gen) % 2 == 0) ? -1 : 1; //Randomly picks from 0 to 1, if 0 -> -1, else 1
            (*current_row_pointer).at(col) = r_val;
        }
    }
    return spins;
}

int calculate_M(const vector<vector<int>> spins){
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
    vector<vector<int>> spins = initialize_spins(L);
    E = calculate_E(L, spins);
    M = calculate_M(spins);
    vector<int> timesteps;
    vector<int> energies;
    // vector<int> energies_squared;
    vector<int> magnetizations;
    // vector<int> magnetizations_squared;
    cout << "Initial energy: " << E << endl;
    // classic_write_to_file(spins, "./data/", "initialized_spins.txt");
    //MC sweeps
    timesteps.push_back(0);
    energies.push_back(E);
    magnetizations.push_back(M);
    for (int t = 1; t < sweeps; t++){
        glauber(L, T, E, M, spins); //Does the changes to the vector globally instead of returning the spins
        // E = pair_energies.first;
        // E_squared = pair_energies.second;
        if (t%save_every == 0){ //Shows 5 sweep values
            // cout << "Sweep " << t << ") \tenergy: " << E << endl;
            //Saving values
            timesteps.push_back(t);
            energies.push_back(E);
            // energies_squared.push_back(E_squared);
            magnetizations.push_back(calculate_M(spins));
            // magnetizations_squared.push_back
        }   
    }

    //Saving values to be analyzed
    cout << "Final energy: " << E << "  after " << sweeps << " sweeps" << endl;
    string directory = "./data/" + to_string(int(T)) + "/";
    classic_write_to_file(timesteps, directory, "timesteps_L="+to_string(L)+"_sweeps="+ to_string(sweeps)+".txt");
    classic_write_to_file(energies, directory, "energies_L="+to_string(L)+"_sweeps="+ to_string(sweeps)+".txt");
    // classic_write_to_file(energies_squared, directory, "energies_squared_L="+to_string(L)+"_sweeps="+ to_string(sweeps)+".txt");
    classic_write_to_file(magnetizations, directory, "magnetizations_L="+to_string(L)+"_sweeps="+ to_string(sweeps)+".txt");
    // classic_write_to_file(magnetizations_squared, directory, "magnetizations_L="+to_string(L)+"_sweeps="+ to_string(sweeps)+".txt");
}

pair<double,double> run_IM_different_temps(const double T, const int L, const int sweeps,const vector<vector<int>> &spins){
    //Initializing
    const int N = pow(L,2);
    vector<vector<int>> temp_spins = spins; //Copying initialized spins
    // const int t_eq = 1000; //To make the system reach equilibrium (tau_eq)
    
    //Values to be calculated
    vector<int> energies;
    vector<int> magnetizations;

    int E = calculate_E(L, spins);
    // int E_squared = pow(E,2); 
    int M = calculate_M(spins);
    // int M_squared = pow(M,2); //Constant no? Just sum of all spins squared, and since all spins are +-1, its just N

    //Used to calculate average observables in the end
    double E_sum = E;
    double E_squared_sum = E*E;
    double M_sum = M;
    double M_squared_sum = M*M;

        //Thermalization to reach equilibration
        // for(int i = 0; i < t_eq; i++){
        //     glauber(L, T, E, E_squared, M, temp_spins); //Does the changes to the vector globally instead of returning the spins
        // }

        //Checking fluctuations
    for(int j = 0; j < sweeps; j++){
        glauber(L,T,E,M,temp_spins); //Right now doesn't make sense to have two different operations, since the first and second are equal
        E_sum += E;
        E_squared_sum += E*E;
        M_sum += M;
        M_squared_sum += M*M;
    }
    //Can be removed later
    double E_mean_sum = E_sum/N;
    double E_squared_mean_sum = E_squared_sum/N;
    double M_mean_sum = M_sum/N;
    double M_squared_mean_sum = M_squared_sum/N;

    //Using ergodicity theorem(adding one sweep because the start values are added to each sum)
    double E_org = E_mean_sum/(sweeps + 1);
    double E_squared_org = E_squared_mean_sum/(sweeps + 1);
    double M_org = M_mean_sum/(sweeps + 1);
    double M_squared_org = M_squared_mean_sum/(sweeps + 1);

    double specific_heat = (E_squared_org - E_org*E_org)/(K_b*T*T)*N; //Gange med N for å gjøre opp for averagen?
    double susceptibility = N*(M_squared_org - M_org*M_org)/(K_b*T); //Er beta kb*T her?

    return pair<double,double> (specific_heat, susceptibility);
    
}