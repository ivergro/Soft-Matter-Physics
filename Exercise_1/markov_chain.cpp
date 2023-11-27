#include "markov_chain.h"

Matrix P_1(){
    Matrix P_1(2,2);
    P_1.set(0,0,0.5);
    P_1.set(0,1,0.5);
    P_1.set(1,0,1.0);
    cout << "P_1 in use:\n " << P_1 << endl;
    return P_1;
}

Matrix P_2(){
    Matrix P_2(3,3);
    P_2.set(0,2,1.);
    P_2.set(1,1,1.);
    P_2.set(2,0,0.25);
    P_2.set(2,2,0.75);
    cout << "P_2 in use:\n " << P_2 << endl;
    return P_2;
}

Matrix P_15(){
    Matrix P_15(3,3);
    P_15.set(0,0,0.5);
    P_15.set(0,1,0.25);
    P_15.set(0,2,0.25);
    P_15.set(1,0,0.5);
    P_15.set(1,2,0.5);
    P_15.set(2,0,0.25);
    P_15.set(2,1,0.25);
    P_15.set(2,2,0.5);
    cout << "P_15 in use:\n" << P_15 << endl;
    return P_15;
}

Matrix P_16(){
    Matrix P_16(2,2);
    P_16.set(0,0,1.);
    P_16.set(1,0,0.5);
    P_16.set(1,1,0.5);
    cout << "P_16 in use:\n" << P_16 << endl;
    return P_16;
}

Matrix P_18(){
    Matrix P_18(3,3);
    P_18.set(0,0,0.5);
    P_18.set(0,1,0.5);
    P_18.set(1,0,0.25);
    P_18.set(1,1,0.5);
    P_18.set(1,2,0.25);
    P_18.set(2,1,0.5);
    P_18.set(2,2,0.5);
    cout << "P_18 in use\n" << P_18 << endl;
    return P_18;
}

void find_periodicity(Matrix prob_matrix, Matrix current_vec, const int N){
    const Matrix start_vec = current_vec;
    cout << "Start vec: " << start_vec << endl;

    vector<int> periods;
    int p = 1;
    for (int i = 0; i < N; i++){
        Matrix prob_vec = current_vec.Transpose()*prob_matrix;
        // cout << "Prob vec: " << prob_vec << endl;
        // cout << "Rows: " << prob_vec.getRows();
        double rv = (double) rand() / RAND_MAX;
        double criterion = 0;
        for (int j = 0; j < prob_vec.getColumns(); j++){
            // cout << "rv: " << rv << endl;
            // cout << "prob " << (prob_vec.get(0, j) + criterion)<<endl;
            if (rv <= (prob_vec.get(0, j) + criterion)){ //Extracting row elements in prob vector, and matching with randomy generated number to move to decired place
                current_vec -= current_vec; //Setting every element to 0
                current_vec.set(j,0,1.); //Setting right position to value 1
                // cout << "New current vec: " << current_vec;
                break;
            }
            criterion += prob_vec.get(0, j); //Increasing value to increase odds of next event happening
        }
        if (current_vec == start_vec){
            periods.push_back(p);
            p = 0;
        }
        ++p;
    }
    cout << "Final vector:\n" << current_vec << endl;
    double mean_period = 0;
    for (int k = 0; k < periods.size(); k++){
        mean_period += periods.at(k);
    }
    mean_period /= periods.size();
    cout << "Mean periode: " << mean_period << endl;
}

void calculate_P_n(Matrix P, const int n){
    // cout << "Before start: " << P;
    for (int i = 2; i <= n; i++){
        Matrix old_P = P;
        P *= P;
        if (isnan(P.get(0,0))){
            cout << "Found nan, result of (P)^n with n=" << i << endl << old_P;
            break;
        } else if (strictly_positive_el(P)){
            cout << "Found an n where al elements are strictly positive n = " << i << "\nMatrix is now:\n" << P;
            cout << "The matrix is therefore regular!\n";
            break;
        }
    }
    
    cout << "Result of (P)^n with n=" << n << endl << P;
}


bool strictly_positive_el(Matrix P){
    for (int row = 0; row < P.getRows(); row++){
        for (int col = 0; col < P.getColumns(); col++){
            if (P.get(row,col) <= 0){
                return false;
            }
        }
    }
    return true; //Never hit a negative element
}