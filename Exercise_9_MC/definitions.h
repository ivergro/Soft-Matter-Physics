#define N_MOVES 1
#define PI 3.141592653589793

typedef struct
{
    double x, y, z;

    double ox, oy, oz;	//old colloid coordinates
    
    double tx, ty, tz;  //temporary colloid coordinates

} Particle;

Particle *parts = NULL; //It is always a good practice to assign a NULL value to a pointer variable in case you do not have an exact address to be assigned.

//defining the System of patchy IPCs properties

typedef struct  
{
    int NPart;
    long int step; //Current step/sweep of the simulation
    long int NSteps, NPrint, Nrestart;      //number of IPCs, number of MC sweeps, frequency of configurations. NPrint is the number of steps between each print of the energy in do_MC()
    int restart;
    long seed;

    int overlap;
    long int tries[N_MOVES];		//array with trial number for different moves
    long int accepted[N_MOVES];		//acceptance for different moves, only one type of move is currently implemented

    double disp_max;		       //maximum allowed displacement for colloids
	
    double box_x, box_y, box_z;
    double T;	                    //T*, reduced units T/Kb
    double energy;
    double pressure;
    double energies[N_MOVES];		//array with energy for different moves
    double pressures[N_MOVES];		//array with pressure for different moves
    
    double sigma;

    int model;			     //switch for models
    int run;                 //run number (Used for several realizations)


} System;

System mySys;

