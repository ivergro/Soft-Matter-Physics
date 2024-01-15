
#define min(a,b)    ({ __typeof__ (a) _a = (a);  __typeof__ (b) _b = (b); _a < _b ? _a : _b; }) //Chooses min of values (equal to floor i guess)





/********************************************************************************************/

/**
 * @brief Should only compute the new energy change of the new particle?
 * 
 * @return double 
 */
double compute_energy_translation(){

    return 0;
}





//E


double energy_one_particle_HS(int i){
    double en = 0.;
    for(int j = 0; j< mySys.NPart; j++){
        if (i <= j) continue;
        else{
            double r = calculate_r(i, j);
            if(r < mySys.sigma){
                //Overlap
                en = 1e10;
                return en;
            }
        }
    }
    return en;
}

double compute_energy_HS(){

    int i;
    double en = 0.;
    long int j;

    // Overlapp (avstand mindre enn sigma), sett energi drit stor
    for(i = 0; i< mySys.NPart; i++){
        en += energy_one_particle_HS(i);
    }
   
    return en;

}


/**
 * @brief Potential below cut off distance
 * 
 * @param r 
 * @return double 
 */
double LJ_pot(double r){
    double epsilon = 1.;
    double V_LJ = 4*epsilon*(pow(mySys.sigma/r, 12) - pow(mySys.sigma/r, 6));
    return V_LJ;
}

double tail_pot(double sigma_cut){
    double density = mySys.NPart/(mySys.box_x*mySys.box_y*mySys.box_z);
    return 8/3 * PI*density*(1/3*pow(mySys.sigma/sigma_cut, 9) - pow(mySys.sigma/sigma_cut, 3));
}

/**
 * @brief Used to compute energy of one particle in the system. Can be used to calculate total energy, or change in energy when moving one particle
 * 
 * @param i particle index
 * @return double 
 */
double energy_one_particle_LJ(int i){
    double en = 0.;
    double sigma_cut = mySys.box_x/2.;
    if (mySys.box_y/2. < sigma_cut || mySys.box_z/2. < sigma_cut) printf("Warning: box_y or box_z is smaller than sigma_cut\n");
    for(int j = 0; j< mySys.NPart; j++){
        if (i <= j) continue;
        else{
            double r = calculate_r(i, j);
            if(r < sigma_cut){
                en += LJ_pot(r);
                // printf("LJ_pot: %f\n", LJ_pot(r));
            } else if( r >= sigma_cut){
                en += tail_pot(sigma_cut);
            } else{
                printf("Error: r is negative\n");
            }
        }
    }
    return en;
}

double compute_energy_LJ(){
    double en = 0.;
    // Overlapp (avstand mindre enn sigma), sett energi drit stor
    for(int i = 0; i< mySys.NPart; i++){
        en += energy_one_particle_LJ(i);
    }
    return en;
}


double compute_energy_somemodel(){
    double en = 0.;
    switch (mySys.model)
    {
        //Hard sphere model
        case 0:
            en = compute_energy_HS();
            break;
        //Lennard-Jones model
        case 1:
            en = compute_energy_LJ();
            break;
        default:
            printf("Error: model not set properly\n");
            break;
    }
    return en;
}




//P
double LJ_pressure(double r){
    double epsilon = 1.;
    double V_LJ = 8*epsilon*(2*pow(mySys.sigma/r, 12) - pow(mySys.sigma/r, 6));
    return V_LJ;
}

double tail_pressure(double sigma_cut){
    double density = mySys.NPart/(mySys.box_x*mySys.box_y*mySys.box_z);
    return 16/3 * PI*density*density*(2/3*pow(mySys.sigma/sigma_cut, 9) - pow(mySys.sigma/sigma_cut, 3));
}

// double pressure_one_particle_LJ(int i){
//     double PV = 0.;
//     if (mySys.box_y/2. < sigma_cut || mySys.box_z/2. < sigma_cut) printf("Warning: box_y or box_z is smaller than sigma_cut\n");
//     for(int j = 0; j< mySys.NPart; j++){
//         if (i < j) continue;
//         else{
//             double r = calculate_r(i, j);
//             if(r < sigma_cut){
//                 PV += LJ_pressure(r);
//             } else if( r >= sigma_cut){
//                 PV += tail_pressure(mySys.sigma);
//             }else{
//                 printf("Error: r is negative\n");
//             }
//         }
//     }
//     return PV;
// }

/**
 * @brief Calculates pressure of system from ideal gas law (pv = NKbT) and LJ potential. 
 * 
 * @return double pressure of entire system 
 */
double tot_pressure(){
    double ideal_gas = mySys.NPart*mySys.T;
    double V = (mySys.box_x*mySys.box_y*mySys.box_z);
    double P = ideal_gas/V; //T = T*=T*Kb
    double sigma_cut = mySys.box_x/2.;
    if (mySys.box_y/2. < sigma_cut || mySys.box_z/2. < sigma_cut) printf("Warning: box_y or box_z is smaller than sigma_cut\n");
    //Adding virial correction term
    for(int i = 0; i< mySys.NPart; i++){
        for(int j = 0; j< mySys.NPart; j++){
            if (i <= j) continue; //Avoiding double counting
            else{
                double r = calculate_r(i, j);
                if(r < sigma_cut){
                    P += LJ_pressure(r)/V;
                } else if( r >= sigma_cut){
                    P += tail_pressure(mySys.sigma);
                }else{
                    printf("Error: r is negative\n");
                }
            }
        }
    }
    return P;
}

