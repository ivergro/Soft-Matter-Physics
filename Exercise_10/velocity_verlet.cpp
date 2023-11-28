#include "velocity_verlet.h"

//1D VV
double update_x(const double x_now, const double v_now, const double f_now, const double dt, const double m=1.){
    return x_now + v_now*dt + f_now*dt*dt/(2*m);
}

double half_kick(const double v_now, const double a_now, const double a_next, const double dt){
    return v_now + 
}

double update_v(const double v_now, const double f_now, const double f_next, const double dt, const double m=1.){
    return v_now + dt/(2*m) * (f_next + f_now);
}