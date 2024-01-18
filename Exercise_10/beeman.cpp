#include "beeman.h"
vector<double> next_r_beeman(const vector<double>& r_now, const vector<double>& v_now, const vector<double>& f_now, const vector<double>& f_prev, const double dt, const double m){
    vector<double> r_next(r_now.size());
    for (int i = 0; i < r_now.size(); ++i) {
        r_next[i] = r_now[i] + v_now[i]*dt + (4.*f_now[i]*dt*dt - f_prev[i])/(6*m)*dt*dt;
    }
    return r_next;
}

vector<double> next_v_beeman(const vector<double>& v_now, const vector<double>& f_next, const vector<double>& f_now, const vector<double>& f_prev, const double dt, const double m){
    vector<double> v_next(v_now.size());
    for (int i = 0; i < v_now.size(); ++i) {
        v_next[i] = v_now[i] + (2.*f_next[i] + 5.*f_now[i]*dt - f_prev[i])/(6.*m)*dt;
    }
    return v_next;
}

vector<double> next_f_beeman(const vector<double>& r_next, const double k){
    vector<double> f_next(r_next.size());
    for (int i = 0; i < r_next.size(); ++i) {
        f_next[i] = -k*r_next[i];
    }
    return f_next;
}

// vector<double> v_dt(const vector<double>& r_0, const vector<double>& v_0, const double dt, const double m){
//     vector<double> vdt(v_0.size());
//     vector <double> f_0 = next_f_beeman(r_0);
//     for (int i = 0; i < v_0.size(); ++i) {
//         vdt[i] = v_0[i] + f_0[i]*dt/(m);
//     }
//     return vdt; 
// }

vector<double> v_dt(const vector<double>& r_0, const vector<double>& r_dt, const double dt){
    vector<double> vdt(r_0.size());
    for (int i = 0; i < vdt.size(); ++i) {
        vdt[i] = (r_dt[i] - r_0[i])/dt;
    }
    return vdt; 
}