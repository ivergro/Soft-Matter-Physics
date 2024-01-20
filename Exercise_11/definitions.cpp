#include "definitions.h"

System::System(int run_LJ): run(run_LJ){
    this->read_input_file(run_LJ);
}



void System::read_input_file(int run_LJ){
    ifstream _myfile;
    _myfile.open("./EX11_equilibration/run"+to_string(run_LJ)+".xyz");
    if (!_myfile.is_open()) {
        cerr << "Error: Unable to create or open the file" << std::endl;
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
    string filepath = "./data/";
    this->save_velocities(filepath);
}

void System::save_velocities(string filepath){
    string filename = "run"+to_string(this->run)+"_velocities.xyz";
    ofstream outfile(filepath + filename);
    if (!outfile.is_open()) {
        cerr << "Error: Unable to create or open the file '" << filename << "'" << " with path '" << filepath << "'" << std::endl;
        if (filesystem::create_directory(filepath)) {
            cout << "Directory '" << filepath << "' created successfully." << endl;
            this->save_velocities(filepath);
        }
    } else{
        //Extract vector comp. of parts first, save each to its own vector, or write to file imidiately
        for (int i = 0; i < this->NPart; i++){
            outfile << this->parts[i].vx << " " << this->parts[i].vy << " " << this->parts[i].vz << "\n";
        }
    }
}