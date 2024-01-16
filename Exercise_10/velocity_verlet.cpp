#include "velocity_verlet.h"

//1D VV

//REMARK: Must be computed before next_v
double next_x(const double x_now, const double v_now, const double f_now, const double dt, const double m){
    return x_now + v_now*dt + f_now*dt*dt/(2*m);
}

//REMARK: Must be computed after next_x and next_f
double next_v(const double v_now, const double f_now, const double f_next, const double dt, const double m){
    return v_now + (f_now + f_next)*dt/(2*m);
}

//Remark: Must be computed after next_v
//Spring model
double next_f(const double x_now, const double x_eq, const double k){
    return -k*(x_now - x_eq);
}

// double half_kick(const double v_now, const double a_now, const double a_next, const double dt){
//     return v_now + 
// }

