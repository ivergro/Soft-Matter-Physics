/**
 * @brief Copy the coordinates of the particle "x" to the old coordinates "ox"
 * 
 * @param idp particle index
 */
void copycoordinates(long int idp)
{
     parts[idp].ox = parts[idp].x;
     parts[idp].oy = parts[idp].y;
     parts[idp].oz = parts[idp].z;
}

/**
 * @brief Rollback the coordinates of the particle "x" to the old coordinates "ox"
 * 
 * @param idp particle index
 */
void rollback(long int idp)
{
     parts[idp].x = parts[idp].ox;
     parts[idp].y = parts[idp].oy;
     parts[idp].z = parts[idp].oz;

}


void translation(long int idp)
{
    //random. Think of what disp_max should be. If disp_max is one box lenght, this code should give periodic bc with max displacement 0.5 
    double displacement_x = ran2(&mySys.seed) * mySys.disp_max; //number between (0, dmax)
    double displacement_y = ran1(&mySys.seed) * mySys.disp_max;
    double displacement_z = ran3(&mySys.seed) * mySys.disp_max;
    double new_x = P_Img((parts[idp].x + displacement_x), mySys.box_x); //P_Img is periodic image function
    double new_y = P_Img((parts[idp].y + displacement_y), mySys.box_y);
    double new_z = P_Img((parts[idp].z + displacement_z), mySys.box_z);
    parts[idp].x = new_x;
    parts[idp].y = new_y;
    parts[idp].z = new_z;

}


/**
 * @brief Should perfrom one sweep of the MC algorithm, consist of N trial moves, where N is the number of particles in the system
 * 
 */
void do_MC_sweep()
{
 /*
    1. Select a particle at random
    2. Propose a displacement in each direction. The maximum displacement should be set as a parameter
    dmax.
    3. Compute the energy of the system before the displacement.
    4. Displace the particle and compute the energy of the system after the displacement
    5. Accept or reject according to the Metropolis rule.
*/

    // 1. Select a particle at random
    long int r_ind = (long int) (ran2(&mySys.seed)*mySys.NPart);

    // printf("Particle %ld\n", r_ind);
    // Save current position
    copycoordinates(r_ind);

    // 2. Propose a displacement in each direction.
    

    // 3. Compute the energy of the system before the displacement.
    double oldEnergy = mySys.energy;
    translation(r_ind);
    // 4. Displace the particle and compute the energy of the system after the displacement
    double newEnergy = compute_energy_somemodel();
    // 5. Accept or reject according to the Metropolis rule.
    if (ran2(&mySys.seed) < exp((oldEnergy - newEnergy)/mySys.T)) //Higher new energy than old energy would lead to a low number, decreasing the chance of acceptance
    {
        // printf("Accepted\n");
        // print_position(r_ind);
        mySys.accepted[0]++;    //Increment number of accepted translations
        mySys.energy = newEnergy;
        if (mySys.model == 1){ mySys.pressure = tot_pressure();}
    }
    else
    {   
        mySys.overlap++;
        rollback(r_ind);
    }
}

void do_MC(){

    char dumpname[100];
    char restartname[100];

    FILE* f = fopen("./output_data/energy.dat", "a");
    FILE* g = fopen("./output_data/acceptance.dat", "a"); 
    FILE* p = fopen("./output_data/pressure.dat", "a");

    sprintf(restartname,"restartpoint.dat");

    
    for(mySys.step=0; mySys.step < mySys.NSteps; mySys.step++){
        mySys.tries[0]++; //Increment number of tries for translation
        do_MC_sweep();
        //if(mySys.step % 1000 == 0)  WriteConf(restartname);
     
        if(mySys.step % mySys.NPrint == 0){ 
            fprintf(f, "%ld. E = %4f\n", mySys.step, mySys.energy);
            fprintf(p, "%ld. P = %4f\n", mySys.step, mySys.pressure);
        }
    }
   
    fclose(f); fclose(g);
   
}

