/**
 * @brief Allocates memory for the particles "parts" vector
 * 
 */
void allocate_()
{
    parts = ( Particle *)malloc(mySys.NPart*sizeof(Particle));   
}

/**
 * @brief Releases memory used by the particles "parts" vector
 * 
 */
void clean_()
{
    free(parts);
    mySys.tries[0] = 0;
    mySys.accepted[0] = 0;
    mySys.overlap = 0;
    mySys.step = 0;
}



// void ReadConf()
// {
//     FILE* fp = fopen(mySys.restart_file, "r"); 
//     if(fp == NULL){ printf("File does not exist!\n"); exit(1); }

//     double a, b, c;

//     for(int i = 0; i < mySys.NPart; i++) 
//     {
//         fscanf(fp, "%lf %lf %lf", &a, &b, &c); 
// 	parts[i].x = a; parts[i].y = b; parts[i].z = c;
//      }
//     fclose(fp); 
// }
/*

void WriteConf(char filename[])
{
    FILE* fp = fopen(filename, "w"); 

    for(int i = 0; i < mySys.NPart; i++) 
    {
        fprintf(fp, "%lf %lf %lf\n", parts[i].x, parts[i].y, parts[i].z); 
    }
    fflush(fp); fclose(fp); // Close file when we're done
}


*/

/**
 * @brief Calculates the minimum distance between two particles. 
 * 
 * @param dx distance between particles
 * @param L System size in 1 direction
 * @return double 
 */
double MinD(double dx, double L){

    double dx1;
    dx1 = dx - rint(dx/L)*L;
    return dx1;
}

/**
 * @brief Keeps particle position inside box
 * 
 * @param z position to be checked
 * @param L box size
 * @return double 
 */
double P_Img (double z, double L){
        
    double z1;
    z1 = z - floor(z/L)*L;
    return z1;
}


//My funcs

void print_model_parameters(){
    printf("N = %d\n", mySys.NPart);
    printf("N_steps = %d\n", mySys.NSteps);
    printf("box_x = %f\n", mySys.box_x);
    printf("box_y = %f\n", mySys.box_y);
    printf("box_z = %f\n", mySys.box_z);
    printf("temperature = %f\n", mySys.T);
    printf("delta_x = %f\n", mySys.disp_max);
    printf("myseed = %d\n", mySys.seed);
    printf("sigma = %f\n", mySys.sigma);
    printf("model = %d\n", mySys.model);
    printf("ratio_samplings = %d\n", mySys.NPrint);
}

void create_directory_if_not_exists(char *dir) {
    char tmp[256];
    char *p = NULL;
    size_t len;

    snprintf(tmp, sizeof(tmp),"%s",dir);
    len = strlen(tmp);
    if(tmp[len-1] == '/')
        tmp[len-1] = 0;
    for(p = tmp + 1; *p; p++)
        if(*p == '/') {
            *p = 0;
            mkdir(tmp);
            *p = '/';
        }
    mkdir(tmp);
}

void save_config(){
    char dir[256];
    if (mySys.model == 0){
        sprintf(dir, "./data2/density=%.2f/dx=%.2fsigma", mySys.NPart/(mySys.box_x*mySys.box_y*mySys.box_z), mySys.disp_max);
    }
    else if (mySys.model == 1){
        sprintf(dir, "./data/LJ/T=%.1f/density=%.2f/dx=%.2fsigma", mySys.T, mySys.NPart/(mySys.box_x*mySys.box_y*mySys.box_z), mySys.disp_max);
    }create_directory_if_not_exists(dir);

    char filename[256];
    sprintf(filename, "%s/run%d.xyz", dir, mySys.run);
    FILE *fp = fopen(filename, "w");

    //Parameters
    fprintf(fp, "N = %d\n", mySys.NPart);
    fprintf(fp, "N_steps = %d\n", mySys.NSteps);
    fprintf(fp, "box_x = %f\n", mySys.box_x);
    fprintf(fp, "box_y = %f\n", mySys.box_y);
    fprintf(fp, "box_z = %f\n", mySys.box_z);
    fprintf(fp, "temperature = %f\n", mySys.T);
    fprintf(fp, "delta_x = %f\n", mySys.disp_max);
    fprintf(fp, "myseed = %d\n", mySys.seed);
    fprintf(fp, "sigma = %f\n", mySys.sigma);
    fprintf(fp, "model = %d\n", mySys.model);
    fprintf(fp, "tries = %ld\n", mySys.tries[0]);
    fprintf(fp, "accepted = %ld\n", mySys.accepted[0]);
    fprintf(fp, "overlap = %d\n", mySys.overlap);
    fprintf(fp, "density = %2f\n", mySys.NPart / (mySys.box_x*mySys.box_y*mySys.box_z));

    //End results
    fprintf(fp, "E = %f\n", mySys.energy);
    fprintf(fp, "P = %f\n", mySys.pressure);
    fprintf(fp, "Lattice={%.2f %.2f %.2f}\n", mySys.box_x, mySys.box_y, mySys.box_z);
    for (int i = 0; i < mySys.NPart; i++)
    {
        fprintf(fp, "pos: %.2f %.2f %.2f\n", parts[i].x, parts[i].y, parts[i].z);
    }
    fclose(fp);

    printf("Saved config to %s\n", dir);
}

void print_positions(){
    for (int i = 0; i < mySys.NPart; i++)
    {
        printf("\tpos: %.2f %.2f %.2f\n", parts[i].x, parts[i].y, parts[i].z);
    }
}

void print_position(int idp){
    printf("\told pos: %.2f %.2f %.2f\n", parts[idp].ox, parts[idp].oy, parts[idp].oz);
    printf("\tpos: %.2f %.2f %.2f\n", parts[idp].x, parts[idp].y, parts[idp].z);
}

void print_metadata(){
    printf("delta x = %2f*sigma\n", mySys.disp_max);
    printf("Density = %2f\n", mySys.NPart/(mySys.box_x*mySys.box_y*mySys.box_z));
}


void set_box_size(int i){
    switch(i) {
        case 0:
            //0.05
            // mySys.box_x = 10;
            // mySys.box_y = 10;
            // mySys.box_z = 20;
            mySys.box_x = pow(2000, 1.0/3.0);
            mySys.box_y = pow(2000, 1.0/3.0);
            mySys.box_z = pow(2000, 1.0/3.0);
            break;
        case 1:
            //0.3
            // mySys.box_x = 3.33;
            // mySys.box_y = 10;
            // mySys.box_z = 10;
            mySys.box_x = pow(333.33, 1.0/3.0);
            mySys.box_y = pow(333.33, 1.0/3.0);
            mySys.box_z = pow(333.33, 1.0/3.0);
            break;
        case 2:
            //0.5
            // mySys.box_x = 5;
            // mySys.box_y = 5;
            // mySys.box_z = 8;
            mySys.box_x = pow(200, 1.0/3.0);
            mySys.box_y = pow(200, 1.0/3.0);
            mySys.box_z = pow(200, 1.0/3.0);
            break;
        case 3:
            //1
            mySys.box_x = 4;
            mySys.box_y = 5;
            mySys.box_z = 5;
            // mySys.box_x = pow(100, 1.0/3.0);
            // mySys.box_y = pow(100, 1.0/3.0);
            // mySys.box_z = pow(100, 1.0/3.0);
            break;
        default:
            fprintf(stderr, "Error: got input %d, box_x, box_y, box_z not set properly\n", i);
            break;
    }
}

void set_disp_max(int i){
    switch(i) {
        case 0:
            //0.01
            mySys.disp_max = 0.01*mySys.sigma;
            break;
        case 1:
            //0.05
            mySys.disp_max = 0.05*mySys.sigma;
            break;
        case 2:
            //0.1
            mySys.disp_max = 0.1*mySys.sigma;
            break;
        case 3:
            //0.5
            mySys.disp_max = 0.5*mySys.sigma;
            break;
        case 4:
            //1
            mySys.disp_max = 1*mySys.sigma;
            break;
        default:
            fprintf(stderr, "Error: disp_max not set properly\n");
            break;
    }
}

double calculate_r(int i, int j){
    double dx = parts[i].x - parts[j].x;
    double dy = parts[i].y - parts[j].y;
    double dz = parts[i].z - parts[j].z;
    dx = MinD(dx, mySys.box_x);
    dy = MinD(dy, mySys.box_y);
    dz = MinD(dz, mySys.box_z);
    double r2 = dx*dx + dy*dy + dz*dz; //r2 = r^2!
    double r = sqrt(r2);
    return r;
}

double* linspace(double a, double b, int n){
    double *array = malloc(n * sizeof(double));
    double step = (b-a)/(n-1);
    for (int i = 0; i < n; i++){
        array[i] = a + i*step;
    }
    return array;
}