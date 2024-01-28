/**
 * @brief Reads input from "param.dat" file and adds the values to the system struct "mySys"
 * 
 */
void read_input_file(int run_LJ)
{
    FILE *_myfile;

    if (run_LJ == 0){
        _myfile = fopen("param.dat", "r");
    } else if(run_LJ == 1){
        _myfile = fopen("param_LJ.dat", "r");
    } else if(run_LJ == 2){
        _myfile = fopen("param_ex11.dat", "r");
    }
    
    
    char line[100];
    int i=0;
    char *token;
    char *_key=NULL;
    char *_value = NULL;

    while(fgets(line, sizeof(line), _myfile) != NULL){

      token = strtok(line, " ");
      i = 0;
      while (token != NULL) {
          if (i == 0) _key = token;
	  if (i == 2) _value = token;
          token = strtok(NULL, " ");
          i++;                          //Should i increment somewhere else? this should pick up the actual value in the data file
      }     
   
      if(strcmp("N",_key) == 0){ mySys.NPart = atoi(_value); continue; }
      if(strcmp("N_steps",_key) == 0){ mySys.NSteps = atoi(_value); continue; }
      if(strcmp("box_x",_key) == 0){ mySys.box_x = atof(_value); continue; } 
      if(strcmp("box_y",_key) == 0){ mySys.box_y = atof(_value); continue; }    //These values below are not added in param.dat
      if(strcmp("box_z",_key) == 0){ mySys.box_z = atof(_value); continue; }
      if(strcmp("temperature",_key) == 0){ mySys.T = atof(_value); continue; }
      if(strcmp("delta_x",_key) == 0){ mySys.disp_max = atof(_value); continue; }
      if(strcmp("myseed",_key) == 0){ mySys.seed = atoi(_value); continue; } 
      if(strcmp("sigma",_key) == 0){ mySys.sigma = atof(_value); continue; }
      if(strcmp("model",_key) == 0){ mySys.model = atoi(_value); continue; }
      if(strcmp("ratio_samplings",_key) == 0){ mySys.NPrint = atoi(_value); continue; }   //---------------------------------

    }
    
    fclose(_myfile);

}




void initialize_particles_randomly() {
    for (int i = 0; i < mySys.NPart; i++) {
        while (1) {
            // Generate random position for the particle
            double x = ran2(&mySys.seed) * mySys.box_x;
            double y = ran2(&mySys.seed) * mySys.box_y;
            double z = ran2(&mySys.seed) * mySys.box_z;

            // Check if the new position overlaps with any existing particle
            int overlap = 0;
            for (int j = 0; j < i; j++) {
                double dx = x - parts[j].x;
                double dy = y - parts[j].y;
                double dz = z - parts[j].z;
                dx = MinD(dx, mySys.box_x);
                dy = MinD(dy, mySys.box_y);
                dz = MinD(dz, mySys.box_z);

                double distance = sqrt(dx*dx + dy*dy + dz*dz);
                if (distance < mySys.sigma) {
                    overlap = 1;
                    break;
                }
            }

            // If there's no overlap, set the particle's position to the new position
            if (!overlap) {
                parts[i].x = x;
                parts[i].y = y;
                parts[i].z = z;
                break;
            }
        }
    }
    int j;
    mySys.energy = compute_energy_somemodel();
    if (mySys.model == 1){ mySys.pressure = tot_pressure();}
    if (mySys.energy > 0.){
      printf("Energy is positive, should reinitializing\n");
        // initialize_particles_grid();
    }
}


void initialize_particles_max_dens(){
  int nx = (int)round(mySys.box_x/mySys.sigma);
  int ny = (int)round(mySys.box_y/mySys.sigma);
  int nz = (int)round(mySys.box_z/mySys.sigma);

  int count = 0;
  //Positioning particle in grid (4,4,5)
  for (int i = 0; i < nx; i++) {
    for (int j = 0; j < ny; j++){
      for (int k = 0; k < nz; k++){
        if (count < mySys.NPart){
          parts[i*nz*ny + j*nz + k].x = i*mySys.sigma;  
          parts[i*nz*ny + j*nz + k].y = j*mySys.sigma;
          parts[i*nz*ny + j*nz + k].z = k*mySys.sigma;
          count++;
        }
      }
    }
  }
  printf("Parts vector consisting of %d particles\n", count);
  mySys.energy = compute_energy_somemodel();
  if (mySys.model == 1){ mySys.pressure = tot_pressure();}
}

void initialize_particles_randomly_max_dens() {
  int init_tries = 0;
  for (int i = 0; i < mySys.NPart; i++) {
    while (1) {
      init_tries++;
      // Generate random position for the particle
      double x = ran2(&mySys.seed) * mySys.box_x;
      double y = ran2(&mySys.seed) * mySys.box_y;
      double z = ran2(&mySys.seed) * mySys.box_z;

      // Check if the new position overlaps with any existing particle
      int overlap = 0;
      for (int j = 0; j < i; j++) {
          double dx = x - parts[j].x;
          double dy = y - parts[j].y;
          double dz = z - parts[j].z;
          dx = MinD(dx, mySys.box_x);
          dy = MinD(dy, mySys.box_y);
          dz = MinD(dz, mySys.box_z);

          double distance = sqrt(dx*dx + dy*dy + dz*dz);
          if (distance < mySys.sigma) {
              overlap = 1;
              break;
          }
      }

      // If there's no overlap, set the particle's position to the new position
      if (!overlap) {
          parts[i].x = x;
          parts[i].y = y;
          parts[i].z = z;
          break;
      } else if (init_tries > 100000){
        printf("Could not initialize particle %d\n", i);
        break;
      }
    }
  }
  mySys.energy = compute_energy_somemodel();
  if (mySys.model == 1){ mySys.pressure = tot_pressure();}
  if (mySys.energy > 0.){
    printf("Energy is positive, should reinitializing\n");
      // initialize_particles_grid();
  }
}