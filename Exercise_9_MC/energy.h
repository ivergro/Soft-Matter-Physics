
#define min(a,b)    ({ __typeof__ (a) _a = (a);  __typeof__ (b) _b = (b); _a < _b ? _a : _b; }) //Chooses min of values (equal to floor i guess)


double compute_energy_somemodel(int i, int j){

    return 0.;
}


/********************************************************************************************/

/**
 * @brief Should only compute the new energy change of the new particle?
 * 
 * @return double 
 */
double compute_energy_translation(){

    return 0;
}


double compute_energy(){

    int i;
    double en = 0.;
    long int index;
    long int ix, iy, iz;
    long int j;

    // Overlapp (avstand mindre enn sigma), sett energi drit stor
    for(i = 0; i< mySys.NPart; i++){
        for(j = 0; j< mySys.NPart; j++){
            if (i == j) continue;
            else{
                double dx = parts[i].x - parts[j].x;
                double dy = parts[i].y - parts[j].y;
                double dz = parts[i].z - parts[j].z;
                dx = MinD(dx, mySys.box_x);
                dy = MinD(dy, mySys.box_y);
                dz = MinD(dz, mySys.box_z);
                double r2 = dx*dx + dy*dy + dz*dz;
                if(r2 < ((mySys.sigma/2)*(mySys.sigma/2))){
                    //Overlap
                    en = 1e10;
                    return en;
                }
            }
        }
    
    }
   
    return en;

}

