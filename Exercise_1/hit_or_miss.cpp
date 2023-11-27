#include "hit_or_miss.h"

double hit_or_miss_rectangle(int N){
    //Seed to get same numbers each time

    vector<double> hits_x;
    vector<double> hits_y;
    vector<double> misses_x;
    vector<double> misses_y;

    int inside_counter = 0;
    int outside_counter = 0;

    int i = 0;
    while (i < N){
        int r_x = rand();
        int r_y = rand();


    // Scale the random integer to a value between (0 - ext) and (1 + ext), and shifting start value to (a-ext)
        double x_i = (static_cast<double>(r_x) / RAND_MAX) * ((b+ext)-(a-ext)) + (a-ext);
        double y_i = (static_cast<double>(r_y) / RAND_MAX) * ((d+ext)-(c-ext)) + (c-ext);
        // cout << x_i << "\t" << y_i << "\n";

        if (x_i >= a && x_i <= b){
            if (y_i >= c && y_i <= d){
                inside_counter++;
                hits_x.emplace_back(x_i);
                hits_y.emplace_back(y_i);
            }else{
                outside_counter++;
                // misses.emplace_back((Point) {x_i,y_i});
                misses_x.emplace_back(x_i);
                misses_y.emplace_back(y_i);
            }
        }else{
                outside_counter++;
                // misses.emplace_back((Point) {x_i,y_i});
                misses_x.emplace_back(x_i);
                misses_y.emplace_back(y_i);
            }
        i++;
    }

    // cout << "# Points inside square: " << inside_counter << "\n"
    //      << "# Points outside square: "<< outside_counter << "\n"
    //      << "Total iterations: " << i;
    double total_area = (b-a + 2*ext)*(d-c + 2*ext);
    double actual_area = (b-a)*(d-c);
    double calculated_area = inside_counter/(double) i * total_area; //percentvise area hit*total area;
    string filepath = "./data/ex_2_1/N=" + to_string(N) + "/";
    classic_write_to_file(hits_x, filepath, "hits_x_rectangle.txt");
    classic_write_to_file(hits_y, filepath, "hits_y_rectangle.txt");

    classic_write_to_file(misses_x, filepath, "misses_x_rectangle.txt");
    classic_write_to_file(misses_y, filepath, "misses_y_rectangle.txt");
    return calculated_area/actual_area;
}

double hit_or_miss_disk(int N){
    //Seed to get same numbers each time


    vector<Point> inside_p_vec;
    vector<Point> outside_p_vec;
    int inside_counter = 0;
    int outside_counter = 0;

    int i = 0;
    while (i < N){
        int r_x = rand();
        int r_y = rand();


    // Scale the random integer to a value between (0 - ext) and (1 + ext), and shifting start value to (a-ext)
        double x_i = (static_cast<double>(r_x) / RAND_MAX) * 2*(r+ext)-(r+ext);
        double y_i = (static_cast<double>(r_y) / RAND_MAX) * 2*(r+ext)-(r+ext);
        // cout << x_i << "\t" << y_i << "\n";

        double r_point = sqrt(pow(x_i,2) + pow(y_i,2));
        // double theta_point = atan(y_i/x_i); //Trenger ikke vinkel her, må bare vite at radiusen er good, siden sirkelen er sentrert
        if (r_point <= r){
            inside_counter++;
            inside_p_vec.emplace_back((Point) {x_i,y_i});
        }else{
            outside_counter++;
            outside_p_vec.emplace_back((Point) {x_i,y_i});
            }
        i++;
    }

    // cout << "# Points inside square: " << inside_counter << "\n"
    //      << "# Points outside square: "<< outside_counter << "\n"
    //      << "Total iterations: " << i;

    double total_area = pow(2*r+2*ext,2);
    double actual_area = M_PI*pow(r,2);
    double calculated_area = inside_counter/(double) i * total_area; //percentvise area hit*total area;
    return calculated_area/actual_area;
}