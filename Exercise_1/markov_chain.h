#pragma once
#include "std_lib_facilities.h"
#include "matrix.h"

// P_1 = {
//     {0.5, 0,5},
//     {1  , 0  }
// };

Matrix P_1();
Matrix P_2();
Matrix P_15();
Matrix P_16();
Matrix P_18();

void find_periodicity(Matrix prob_matrix, Matrix current_vec, const int N = 100);

void calculate_P_n(Matrix P, const int N);

bool strictly_positive_el(Matrix P);