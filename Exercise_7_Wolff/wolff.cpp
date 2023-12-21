#include "wolff.h"

constexpr int J = 1;
constexpr int k_b = 1;
mt19937 metropolis_gen(1); // Seed for Metropolis function


//Public functions
IsingModel::IsingModel(int L, double T) : L(L), T(T), spins(L, vector<int>(L)), cluster(L, vector<bool>(L)), gen(random_device{}()), dis(0, 1) {
    for (auto& row : spins) {
        for (auto& spin : row) {
            spin = dis(gen) < 0.5 ? 1 : -1;
        }
    }
    E = calculate_E();
    M = calculate_M();
    energies.push_back(E);
    magnetizations.push_back(M);
}

//Or sweep
void IsingModel::step() {
    int i = gen() % L;
    int j = gen() % L;
    cluster = vector<vector<bool>>(L, vector<bool>(L));
    growCluster(i, j, spins[i][j]);
    cluster_sizes.push_back(current_cluster_size);
    current_cluster_size = 0; //Reset cluster sizes
    E = calculate_E(); //May be time consuming
    energies.push_back(E);
    magnetizations.push_back(M);
}

void IsingModel::save(const string& filename, bool final_save) {
    ofstream file(filename, ios_base::app);
    if (!file.is_open()) {
        cout << "Failed to open file: " << filename << endl;
    } else{
        for (const auto& row : spins) {
            for (size_t i = 0; i < row.size(); ++i) {
                file << row[i];
                if (i != row.size() - 1) {
                    file << ",";
                }
                // string data = to_string(row[i]) + ",";
                // file.write(data.c_str(), data.size());
            }
            file << "\n"; // Delimiter between configurations, but not for the final one!
        }
        file << "\n";  
    }
    file.flush();
    file.close();
}

void IsingModel::save_cluster_sizes(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Failed to open file: " << filename << endl;
    } else {
        for (auto it = cluster_sizes.begin(); it != cluster_sizes.end(); ++it) {
            file << *it;
            if (it != prev(cluster_sizes.end())) {
                file << ",";
            }
        }
    }
    file.close();
}

void IsingModel::save_magnetizations(const string& filename){
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Failed to open file: " << filename << endl;
    } else {
        for (auto it = magnetizations.begin(); it != magnetizations.end(); ++it) {
            file << *it;
            if (it != prev(magnetizations.end())) {
                file << ",";
            }
        }
    }
    file.close();        
}

void IsingModel::save_energies(const string& filename){
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Failed to open file: " << filename << endl;
    } else {
        for (auto it = energies.begin(); it != energies.end(); ++it) {
            file << *it;
            if (it != prev(energies.end())) {
                file << ",";
            }
        }
    }
    file.close();        
}

const double IsingModel::get_T() const {
    return T;
}

void IsingModel::set_T(const double T){
    this->T = T;
}

const double IsingModel::get_E() const{
    return E;
}


//Private functions

void IsingModel::growCluster(int i, int j, int spin) {
    if (cluster[i][j] || spins[i][j] != spin) 
        return; //If already flipped, true state, or if spin is opposite of original-> return, don't add anymore
    cluster[i][j] = true; //Adding to cluster
    ++current_cluster_size; //Increasing cluster size
    spins[i][j] = -spins[i][j]; //Changing spin
    M += 2 * spins[i][j];
    if (dis(gen) < 1 - exp(-2 / T)) {
        growCluster((i + 1) % L, j, spin);
        growCluster((i + L - 1) % L, j, spin);
        growCluster(i, (j + 1) % L, spin);
        growCluster(i, (j + L - 1) % L, spin);
    }
}

int IsingModel::calculate_E(){
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

int IsingModel::calculate_M(){
    int sum = 0;
    for (int row = 0; row < L; row++) {
        for (int col = 0; col < L; col++) {
            sum++;
        }
    }
    return sum;
}


//--------------------------------------------------------

//Multiple Monte Carlo

//Function for comparing to IM, and spitting out if they should swap or not
bool swap_condition(const IsingModel* low_temp_model, const IsingModel* high_temp_model){
    //TODO Check condition, which is correct?
    //Remark, temp is in objects
    const double beta_exp = 1/(k_b*(high_temp_model->get_T())) - 1/(k_b*(low_temp_model->get_T()));
    const double energy_exp = high_temp_model->get_E() - low_temp_model->get_E();
    const double metropolis = min(1., exp(beta_exp * energy_exp));
    //Random number validation
    uniform_real_distribution<double> distribution(0.0, 1.0); //Creates a uniform distribution from 0 to 1
    double p = distribution(metropolis_gen);
    return p <= metropolis;
}

void update_positions(vector<IsingModel*> &I_vec, const int k){
    //Change of T
    const double high_T = (I_vec.at(k+1))->get_T();
    const double low_T =  I_vec.at(k)->get_T();
    I_vec.at(k+1)->set_T(low_T);
    I_vec.at(k)->set_T(high_T);
    //Change of position
    IsingModel* temporary_IM = I_vec.at(k+1); //Probably takes up alot of space
    I_vec.at(k+1) = I_vec.at(k);
    I_vec.at(k) = temporary_IM;
}

//Function to loop over all temperature configurations, and checking if adjacent spin config should change temp config
//If k swaps with k + 1, should k+2 only be checked with k + 3?
//Take in a vector with pointers to the IM models
//Measure how often swaps happen to see if model works good. Save in global variable?
void swap_temp_configurations(vector<IsingModel*> &I_vec, int &num_swaps){
    bool previous_swapped = false;
    for (int k = 0; k < I_vec.size() - 1; k++){ //Ignoring last elemnt, since it can only swap with its previous
        if (previous_swapped){ //Skip this k if it was recently swapped
            previous_swapped = false;
        } else{
            if (swap_condition(I_vec.at(k), I_vec.at(k+1))){
                // swap() //make copy constructor to make a smood swap
                update_positions(I_vec, k);
                previous_swapped = true;
                num_swaps++; //Counting globablly number of swaps
            }
        }

    }
}

//---------------------------------------------

//Help func
vector<double> linspace(double start, double end, int numPoints) {
    vector<double> result;
    if (numPoints <= 1) {
        result.push_back(start);
        return result;
    }
    
    double step = (end - start) / (numPoints - 1);
    for (int i = 0; i < numPoints; ++i) {
        result.push_back(start + i * step);
    }
    return result;
}
vector<int> linspace_int(int start, int end, int numPoints) {
    vector<int> result;
    if (numPoints <= 1) {
        result.push_back(start);
        return result;
    }
    
    int step = (end - start) / (numPoints - 1);
    for (int i = 0; i < numPoints; ++i) {
        result.push_back(start + i * step);
    }
    return result;
}