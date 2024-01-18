#include "std_lib_facilities.h"
#include "euler.h"
#include "velocity_verlet.h"
#include "verlet.h"
#include "beeman.h"

string print_vec(vector<double> vec){
    string s = "";
    for (int i = 0; i < vec.size(); ++i) {
        s += to_string(vec[i]) + " ";
    }
    return s;
}

int main(){
    cout << "1) Comparision of Euler integrators\n"
         << "2) Velocit Verlet\n"
         << "3) Verlet\n"
         << "4) Beeman\n"
         << endl;
    int input;
    cin >> input;

    switch(input){
        case 1:{
            first_euler();
            second_euler();
            analytical_harmonic_oscillator();
            break;
        } case 2:{
            //IC
            double x_now = 5.;
            double v_now = 0.; //Should it be calculated from the IC?
            double f_now = next_f(x_now);
            double dt = 0.1;
            double T = 10;
            double t = 0;
            vector<double> xs {x_now};
            vector<double> vs {v_now};
            vector<double> fs {f_now};
            vector<double> ts {t};
            while (t < T){
                cout << t << ". x=" << x_now << " v=" << v_now << " f= " << f_now << endl;
                x_now = next_x(x_now, v_now, f_now, dt);
                v_now = next_v(v_now, f_now, next_f(x_now), dt); 
                f_now = next_f(x_now);
                t += dt;
                xs.push_back(x_now);
                vs.push_back(v_now);
                fs.push_back(f_now);
                ts.push_back(t);
            }
            string filepath = "./data/ex_2/vv/"+to_string(dt)+"/";
            write_to_file(xs, filepath, "x.dat");
            write_to_file(vs, filepath, "v.dat");
            write_to_file(fs, filepath, "f.dat");
            write_to_file(ts, filepath, "t.dat");  

            break;
        } case 3:{
            //IC
            vector<double> r_now {5.};
            vector<double> v_now {0.};
            vector<double> f_now = next_f_verlet(r_now);
            double dt = 0.1;
            double T = 10;
            double t = 0;
            vector<vector<double>> rs {r_now};
            vector<vector<double>> vs {v_now};
            vector<vector<double>> fs {f_now};
            vector<double> ts {t};
            //Helping var. for calculating r and v
            vector<double> r_prev = r_now;
            vector <double> r_prevprev (r_now.size());
            // vector<double> r_half = r_half_kick(r_now, v_now, f_now, dt);

            //extra IC/first helping step
            r_now = r_dt(r_now, v_now, dt);
            v_now = v_now_verlet(r_now, r_prev, dt/2.); //Approx of v
            f_now = next_f_verlet(r_now);
            // v stays the same over this run? could use r_now and r_prev instead of 2dt steps
            t += dt;

            rs.push_back(r_now);
            vs.push_back(v_now);
            fs.push_back(f_now);
            ts.push_back(t);
            
            while (t < T){
                cout << t << ". r=" << print_vec(r_now) << " v=" << print_vec(v_now) << " f= " << print_vec(f_now) << endl;
                //Deep copying
                r_prevprev = vector<double>(r_prev.begin(), r_prev.end());
                r_prev = vector<double>(r_now.begin(), r_now.end());
                //Updating values
                // r_half = r_half_kick(r_now, v_now, f_now, dt);
                r_now = next_r(r_now, r_prev, f_now, dt);
                v_now = v_now_verlet(r_now, r_prevprev, dt);
                f_now = next_f_verlet(r_now);
                t += dt;
                //Saving values
                rs.push_back(r_now);
                vs.push_back(v_now);
                fs.push_back(f_now);
                ts.push_back(t);
            }
            string filepath = "./data/ex_2/verlet/"+to_string(dt)+"/";
            write_vecofvec_to_file(rs, filepath, "r.dat");
            write_vecofvec_to_file(vs, filepath, "v.dat");
            write_vecofvec_to_file(fs, filepath, "f.dat");
            write_to_file(ts, filepath, "t.dat");  
            break;
        } case 4:{
            //IC
            vector<double> r_now {5.};
            vector<double> v_now {0.};
            vector<double> f_now = next_f_beeman(r_now);
            double dt = 0.1;
            double T = 10;
            double t = 0;
            vector<vector<double>> rs {r_now};
            vector<vector<double>> vs {v_now};
            vector<vector<double>> fs {f_now};
            vector<double> ts {t};
            //Helping var. for calculating r and v
            vector<double> f_prev = f_now;
            vector <double> f_prevprev (f_now.size());
            vector <double> rdt = r_dt(r_now, v_now, dt); //Using r_dt for verlet algorithm
            // vector<double> r_half = r_half_kick(r_now, v_now, f_now, dt);

            //extra IC/first helping step
            r_now = vector<double>(rdt.begin(), rdt.end()); 
            f_now = next_f_beeman(r_now); 
            v_now = v_dt(rs[0], rdt, dt); //Approx of v
            // v stays the same over this run? could use r_now and r_prev instead of 2dt steps
            t += dt;

            rs.push_back(r_now);
            vs.push_back(v_now);
            fs.push_back(f_now);
            ts.push_back(t);


            //Approximate r(dt) since f(-dt) isn't defined
            //Then calculate f(dt) from r
            //Approximate v(dt), since we dont have f(-dt)
            
            while (t < T){
                cout << t << ". r=" << print_vec(r_now) << " v=" << print_vec(v_now) << " f= " << print_vec(f_now) << endl;
                f_prevprev = vector<double>(f_prev.begin(), f_prev.end());
                f_prev = vector<double>(f_now.begin(), f_now.end());

                r_now = next_r_beeman(r_now, v_now, f_now, f_prev, dt);
                f_now = next_f_beeman(r_now);
                v_now = next_v_beeman(v_now, f_now, f_prev, f_prevprev, dt);
                t += dt;

                //Saving values
                rs.push_back(r_now);
                vs.push_back(v_now);
                fs.push_back(f_now);
                ts.push_back(t);
            }
            string filepath = "./data/ex_2/beeman/"+to_string(dt)+"/";
            write_vecofvec_to_file(rs, filepath, "r.dat");
            write_vecofvec_to_file(vs, filepath, "v.dat");
            write_vecofvec_to_file(fs, filepath, "f.dat");
            write_to_file(ts, filepath, "t.dat");  
            break;
        }
        default:{
            cout << "Invalid input" << endl;
            break;
        }
    }


    
    return 0;
}


