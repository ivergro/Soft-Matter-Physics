#include "definitions.h"

System::System(int run_LJ, double rc): run(run_LJ), r_cut(rc){
    this->read_input_file(run_LJ);
    this->draw_velocities();
    this->update_f(); //Calculates initial forces
    cout << parts[0].fx << endl;
    cout << parts[0].fy << endl;
    cout << parts[0].fz << endl;
}



void System::read_input_file(int run_LJ){
    ifstream _myfile;
    _myfile.open("./EX11_equilibration/run"+to_string(run_LJ)+".xyz");
    if (!_myfile.is_open()) {
        cerr << "Error: Unable to create or open the file" << endl;
        exit(1);
    }
    string line;
    while(getline(_myfile, line)){
        istringstream iss(line);
        string _key, _value1, _value2, _value3;
        iss >> _key >> _value1 >> _value2 >> _value3;  //Value2 often is =

        if(_key == "N"){ this->NPart = stoi(_value2); continue; }
        if(_key == "N_steps"){ this->NSteps = stoi(_value2); continue; }
        if(_key == "box_x"){ this->box_x = stod(_value2); continue; }
        if(_key == "box_y"){ this->box_y = stod(_value2); continue; }
        if(_key == "box_z"){ this->box_z = stod(_value2); continue; }
        if(_key == "temperature"){ this->T = stod(_value2); continue; }
        if(_key == "myseed"){ this->seed = stoi(_value2); continue; }
        if(_key == "sigma"){ this->sigma = stod(_value2); continue; }
        if(_key == "ratio_samplings"){ this->NPrint = stoi(_value2); continue; }
        if(_key == "pos:"){ 
            Particle p;
            p.x = stod(_value1),2;
            p.y = stod(_value2),2;
            p.z = stod(_value3),2;      
            this->parts.push_back(p);
            continue;
        }
    }

    _myfile.close();
}






void System::draw_velocities(){
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<> d(0, sqrt(this->T));

    vector<double> totalMomentum = {0, 0, 0};
    for (int i = 0; i < NPart; ++i) {
        double vx_0 = d(gen);
        double vy_0 = d(gen);
        double vz_0 = d(gen);
        this->parts[i].vx = vx_0;
        this->parts[i].vy = vy_0;
        this->parts[i].vz = vz_0;
        totalMomentum[0] += vx_0;
        totalMomentum[1] += vy_0;
        totalMomentum[2] += vz_0;
    }

    // Enforce total momentum to be zero
    for (int i = 0; i < NPart; ++i) {
        this->parts[i].vx -= totalMomentum[0] / NPart;
        this->parts[i].vy -= totalMomentum[1] / NPart;
        this->parts[i].vz -= totalMomentum[2] / NPart;
    }
}

void System::save_config(){
    string filepath = "./data/r_cut="+to_string(r_cut)+"/";
    this->save_components(filepath, 'x');
    this->save_components(filepath, 'v');
    this->save_components(filepath, 'f');
}

void System::save_components(string filepath, char component){
    string filename = "run"+to_string(this->run)+"_"+component+".xyz";
    ofstream outfile(filepath + filename);
    if (!outfile.is_open()) {
        cerr << "Error: Unable to create or open the file '" << filename << "'" << " with path '" << filepath << "'" << endl;
        if (filesystem::create_directory(filepath)) {
            cout << "Directory '" << filepath << "' created successfully." << endl;
            this->save_components(filepath, component);
        }
    } else{
        switch(component){
            case 'x':{
                for (int i = 0; i < this->NPart; i++){
                    outfile << this->parts[i].x << " " << this->parts[i].y << " " << this->parts[i].z << "\n";
                }
                break;
            } case 'v':{
                for (int i = 0; i < this->NPart; i++){
                    outfile << this->parts[i].vx << " " << this->parts[i].vy << " " << this->parts[i].vz << "\n";
                }
                break;
            } case 'f':{
                for (int i = 0; i < this->NPart; i++){
                    outfile << this->parts[i].fx << " " << this->parts[i].fy << " " << this->parts[i].fz << "\n";
                }
                break;
            } default:{
                cout << "Error: Invalid mode '" << component << endl;
                break;
            }
        }
    }
}

//Setters/Getters
void System::set_r_cut(double rc){
    this->r_cut = r_cut;
}

double System::get_sigma(){
    return this->sigma;
}


//Updates 

void System::update_r(const double dt){
    double m = parts[0].m;
    for (auto p = this->parts.begin(); p != this->parts.end(); ++p) {
        p->x = P_Img(p->x + p->vx*dt + p->fx*dt*dt/(2*m), box_x);
        p->y = P_Img(p->y + p->vy*dt + p->fy*dt*dt/(2*m), box_y);
        p->z = P_Img(p->z + p->vz*dt + p->fy*dt*dt/(2*m), box_z);
    }
}

/**
 * @brief Calculates the forces between all particles and updates the forces for both particles at one timestep. 
 * Force vectors are fixed before looping
 * 
 */
void System::update_f(){
    // Saving current forces
    this->prepare_f();
    
    for (auto p = this->parts.begin(); p != this->parts.end(); ++p) {
        for (auto p2 = next(p); p2 != this->parts.end(); ++p2) {
            double dx = MinD(p2->x - p->x, box_x);
            double dy = MinD(p2->y - p->y, box_y);
            double dz = MinD(p2->z - p->z, box_z);
            double r2 = dx*dx + dy*dy + dz*dz;
            if (r2 > r_cut*r_cut) continue;
            else{
                double r6 = pow(r2, 3);
                double r12 = pow(r2, 6);
                double r = sqrt(r2);
                double f = 24*epsilon*(2*(sigma/r12) - (sigma/r6))/r;
                if (f > 1000) {
                    f=1000;
                }
                //normalizing and adding to the total force
                p->fx -= f*dx/r; 
                p->fy -= f*dy/r;
                p->fz -= f*dz/r;
                //Opposite force
                p2->fx += f*dx/r; 
                p2->fy += f*dy/r;
                p2->fz += f*dz/r;
            }
        }
    }
}

void System::prepare_f(){
    for (auto &p : this->parts) {
        //saving old forces
        p.ofx = p.fx;
        p.ofy = p.fy;
        p.ofz = p.fz;
        //Clearing current forces before next_f
        p.fx = 0;
        p.fy = 0;
        p.fz = 0;
    }
}

void System::update_v(const double dt){
    double m = parts[0].m;
    for (auto p = this->parts.begin(); p != this->parts.end(); ++p) {
        p->vx += (p->ofx + p->fx)*dt/(2*m);
        p->vy += (p->ofy + p->fy)*dt/(2*m);
        p->vz += (p->ofz + p->fz)*dt/(2*m);
    }
}


//Main run func
void System::run_MD(){

}