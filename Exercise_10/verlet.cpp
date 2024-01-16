#include "verlet.h"


vector<double> next_r(const vector<double>& r_now, const vector<double>& r_prev, const vector<double>& a_now, const double dt){
    vector<double> r_next(r_now.size());
    for (int i = 0; i < r_now.size(); ++i) {
        r_next[i] = 2*r_now[i] - r_prev[i] + a_now[i]*dt*dt;
    }
    return r_next;
}

//Misleading name, calculates forces for current position
vector<double> next_f_verlet(const vector<double>& r_now, const double k){
    // Implement the function here
    vector <double> r_eq(r_now.size(), 0);
    vector<double> f_next(r_now.size());
    for (int i = 0; i < r_now.size(); ++i) {
        f_next[i] = -k*(r_now[i] - r_eq[i]);
    }
    return f_next;
}

//Notice r_next-r_prev, not r_now
vector<double> v_now_verlet(const vector<double>& r_next, const vector<double>& r_prev, const double dt){
    vector<double> v(r_next.size());
    for (int i = 0; i < r_next.size(); ++i) {
        v[i] = (r_next[i] - r_prev[i])/(2*dt);
    }
    return v;
}

//Used to calculate first r movement since we cannot use standard I.C.
vector<double> r_dt(const vector<double>& r_0, const vector<double>& v_0, const double dt, const double m){
    vector<double> rdt(r_0.size());
    vector <double> f_0 = next_f_verlet(r_0);
    for (int i = 0; i < r_0.size(); ++i) {
        rdt[i] = r_0[i] + v_0[i]*dt + f_0[i]*dt*dt/(2*m);
    }
    return rdt; 
}