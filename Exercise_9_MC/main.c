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

//One run
    // read_input_file();
    // printf("Parameters:\n");
    // print_model_parameters();


    // allocate_();    //Allocates memory for the particles "parts" vector
    // // ReadConf();	//Fyll ut param.data
    // //    or
    // // initialize();   //Må lage egen initialize fil
    
    // initialize_particles();
    // printf("Particles initialized\n");

    // do_MC();
    // save_config();
    
    // // Release memory used by particles, it is not needed anymor
    // clean_();
//Data sampling
    for (int j = 0; j < 4; j++){
        //Setting box volume
        read_input_file();
        set_box_size(j);
        for (int k = 0; k < 5; k++){
            set_disp_max(k);
            //Running 10 realizations
            for (int i = 0; i < 10; i++){
                allocate_();    //Allocates memory for the particles "parts" vector
                print_metadata();

                printf("Initializing\n");
                initialize_particles();

                printf("Running MC sim\n");
                mySys.run = i;
                do_MC();
                save_config();
                // Release memory used by particles and resets mySys
                clean_();
            }
        }
    }
    return 0;
}
// gcc -o ../main main.c -lm

// Simulere slik som beskrevet i oppgaven
//rho = 1 tar for lang tid å initialisere, mulig jeg må hjelpe til litt
