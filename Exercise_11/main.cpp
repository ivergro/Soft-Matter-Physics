#include "definitions.h"
#include "utilities.h"
#include "std_lib_facilities.h"

int main(){
    cout << "Creating system\n";
    //sigma = 1 for sim
    double sigma = 1;
    double r_cut_0 = pow(2, 1.0/6.0)*sigma;
    double r_cut_max = 4*sigma;
    double dr_cut = 0.2*sigma;
    double dt = 0.001;
    double T = 1;
    for (double r_cut = r_cut_0; r_cut < r_cut_max; r_cut += dr_cut){
        r_cut = round(r_cut*10)/10;
        cout << "r_cut = " << r_cut << endl;
        System sys(1, r_cut);
        for (double t=0.0; t < T; t += dt){
            sys.update_v(dt);
            sys.update_r(dt);
            sys.update_f();
            
        }
        sys.save_config();
    }
    return 0;
}


//Skjer noe rart med run 1
//Alle får nan verdier etterhvert, perche?
//Partiklene flytter ikke på seg!
//Kan evt lagre totale krefter (uten retning for å unngå 0) per tidssteg, og plotte i python