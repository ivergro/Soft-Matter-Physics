#include "definitions.h"
#include "utilities.h"
#include "std_lib_facilities.h"

int main(){
    cout << "Creating system\n";
    System sys(1);
    cout << "Drawing velocities\n";
    sys.draw_velocities();
    cout << "Saving config\n";
    sys.save_config();
    cout << "Finished\n";



    return 0;
}