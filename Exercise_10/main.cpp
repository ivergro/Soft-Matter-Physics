#include "std_lib_facilities.h"
#include "euler.h"

int main(){
    cout << "1) Comparision of Euler integrators\n"
         << "2) Velocit Verlet\n"
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

            break;
        }
    }


    
    return 0;
}