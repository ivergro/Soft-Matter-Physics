#include "velocity_verlet.h"

double update_r(const double r_now, const double v_now, const double a_now, const double dt){
    return r_now + v_now*dt + a_now*dt*dt/2;
}

double half_kick(const double v_now, const double a_now, const double a_next, const double dt){
    return v_now + 
}