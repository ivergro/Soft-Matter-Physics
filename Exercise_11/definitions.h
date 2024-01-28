#pragma once
#include "std_lib_facilities.h"
#include "utilities.h"


//Reuse some of ex8 code
struct Particle
{
    double x, y, z;     //colloid coordinates
    double vx, vy, vz;	//colloid velocities
    double fx, fy, fz = 0;	//colloid forces
    double ofx, ofy, ofz;	//colloid old forces, used to calculate v(t+dt)

    const double m = 1.0; //mass of the particle
};

//defining the System
class System
{
    int NPart;
    long int step; //Current step/sweep of the simulation
    long int NSteps, NPrint, Nrestart;      //number of IPCs, number of MC sweeps, frequency of configurations. NPrint is the number of steps between each print of the energy in do_MC()
    int restart;
    long seed;

    double box_x, box_y, box_z;
    double T;	                    //T*, reduced units T/Kb
    // double tot_energy;
    // double energies[N_MOVES];		//array with energy for different moves
    
    double sigma = 1;
    double epsilon = 1;
    double r_cut;

    int run;                 //run number (Used for several realizations)

    vector<Particle> parts; //vector of particles

    private:
        void read_input_file(int run_LJ);
        void save_components(string filepath, char component);
    public:
        System(int run_LJ, double rc);
        void draw_velocities();
        void save_config();

        void set_r_cut(double rc);
        double get_sigma();


        void update_r(const double dt);
        void update_v(const double dt);
        void update_f();
        void prepare_f();

        void run_MD();
};
