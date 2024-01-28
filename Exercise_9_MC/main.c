#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> //Used for mkdir function
#include <sys/types.h> //-------||---------




#include "random.h"
#include "definitions.h"
#include "utils.h"
#include "energy.h"
#include "init.h"
#include "MC.h"

int main(int argc, char* argv[] ) //start with command line options. char*argv[] means this is an array of indetermined size of pinters to char, in other words an array of strings.argv is a pointer to string array holding the actual parameters.
{
    printf("1) Single run from param.dat file\n");
    printf("2) Data sampling over several densities\n");
    printf("3) Simple cubic crystal case\n");
    printf("4) Lennard Jones potential\n");
    printf("5) Lennard Jones potential for several densities\n");
    printf("6) Equilibration for ex 11, LJ model\n");
    printf("Enter choice: ");
    int choice;
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
    //One run
        read_input_file(0);
        printf("Parameters:\n");
        print_model_parameters();


        allocate_();    //Allocates memory for the particles "parts" vector
        // ReadConf();	//Fyll ut param.data
        //    or
        // initialize();   //Må lage egen initialize fil
        
        initialize_particles_randomly();
        printf("Particles initialized\n");

        do_MC();
        save_config();
        
        // Release memory used by particles, it is not needed anymor
        clean_();
        break;
    case 2:
    //Data sampling
        for (int j = 0; j < 4; j++){
            //Setting box volume

            read_input_file(0);
            set_box_size(j);
            for (int k = 0; k < 5; k++){
                set_disp_max(k);
                //Running 10 realizations
                for (int i = 0; i < 10; i++){
                    allocate_();    //Allocates memory for the particles "parts" vector
                    print_metadata();

                    printf("Initializing\n");
                    if (j != 3){
                        initialize_particles_randomly();
                    }
                    else{
                        initialize_particles_randomly_max_dens();
                    }
                    

                    printf("Running MC sim\n");
                    mySys.run = i;
                    do_MC();
                    save_config();
                    // Release memory used by particles and resets mySys
                    clean_();
                }
            }
        }
        break;
    case 3:
        printf("Implemented in case 2\n");
        break;
    case 4:
        printf("Lennard Jones model\n");
        int run_LJ = 1;
        read_input_file(run_LJ);
        printf("Parameters:\n");
        print_model_parameters();


        allocate_();    //Allocates memory for the particles "parts" vector
        // ReadConf();	//Fyll ut param.data
        //    or
        // initialize();   //Må lage egen initialize fil
        
        initialize_particles_randomly();
        printf("Particles initialized\n");
        printf("Initial energy: %f\n", mySys.energy);
        printf("Initial pressure: %f\n", mySys.pressure);
        do_MC();
        save_config();
        
        // Release memory used by particles, it is not needed anymor
        clean_();
        break;
    case 5:
        printf("Lennard Jones model for several densities\n");
        read_input_file(1);
        printf("Parameters:\n");
        print_model_parameters();
        long int array_rho_size = 25;
        double* array_rho = linspace(0.01, 0.9, array_rho_size);
        for (int i = i; i < array_rho_size; i++){
            //Setting box sizes
            double box_size = (double) pow(mySys.NPart / array_rho[i], 1./3.); //rho goes to 1.0
            mySys.box_x = box_size;
            mySys.box_y = box_size;
            mySys.box_z = box_size;
            for (int j = 0; j < 5; j++){
                allocate_();    //Allocates memory for the particles "parts" vector
                print_metadata();

                printf("Initializing\n");
                initialize_particles_randomly();            
                

                printf("Running MC sim\n");
                mySys.run = j;
                do_MC();
                save_config();
                // Release memory used by particles and resets mySys
                clean_();
            }
        }
    case 6:
        printf("Equilibration for ex 11, LJ model");
        read_input_file(2);
        printf("Parameters:\n");
        print_model_parameters();
        for (int i = 0; i < 10; i++){
            allocate_();    //Allocates memory for the particles "parts" vector
            print_metadata();

            printf("Initializing\n");
            initialize_particles_randomly();            
            

            printf("Running MC sim\n");
            mySys.run = i;
            do_MC();
            save_config();
            // Release memory used by particles and resets mySys
            clean_();
        }
        break;
    default:
        printf("Invalid choice\n");
        break;
    }

    return 0;
}
// gcc -o ../main main.c -lm

//Include LJ pot
//Should i have kinetic energy as well? regarding the temperature. No dont think so
//Should the temp have anything to say on the metropolis acceptance? Yes i think so
//Pressure is for what?

//Run MC with LJ pot, calculate P on the way. Save values to vec? or just calculate after equilibration of several models
//Look up pressure density plane, see notes MD1 on bottom


//energy_one_particle_LJ doesnt return any value, but tail LJ works
//Hvorfor bruke LJ tail, når partiklene aldri er langt unna hverandre? typ 0.5 box size. Får veldig få opp så langt unna, merkelig?
//Alt funka før jeg endra til å regne energi for en partikkel