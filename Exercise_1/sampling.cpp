#include "sampling.h"

//Ex 2.2
double inversion_method(int n){
    double rv = (double) rand() / RAND_MAX; //To get between 0 and 1
    double pow_law = F_inv(rv, n);
    return pow_law;
}


double F_inv(double x, int n){
    return pow(x,1.0/(n+1));
}

//Fixed function
double extra_inversion(){
    double rv = (double) rand() / RAND_MAX; //To get between 0 and 2
    double pow_law = 2*pow(rv,1.0/3.0); //Inv of of CDF of c*x**2, normalized.
    return pow_law;
}


//Ex 2.3
double random_r(){
    double rv = (double) rand() / RAND_MAX;
    return rv;
}

double random_theta(){
    double rv = (double) rand() / RAND_MAX;
    return 2*M_PI*rv;
}

pair<double, double> improved_circle_coord(){
    double r = pow(random_r(), 1.0/2.0);
    double theta = random_theta();
    return pair<double,double> (r,theta);
}

pair<double, double> Box_Muller_transformation(){
    double r_unif = 1.;
    double theta_unif = 1.;
    //Avoiding inf possibilities
    while(r_unif == 1 || theta_unif == 1){
        r_unif = (double) rand() / RAND_MAX; 
        theta_unif = (double) rand() / RAND_MAX;
        cout << "Happened!" << endl;
    }
    double x = pow(-2*log(1 - r_unif), 1./2.)*cos(2*M_PI*theta_unif);
    double y = pow(-2*log(1 - r_unif), 1./2.)*sin(2*M_PI*theta_unif);
    return pair<double,double> (x,y);
}

//Ex 2.3.1
double p_rej(const double x){return 2.*pow(1./M_PI,1./2.)*exp(-x*x);}

double g_rej(const double x, const double p){
    const double A = 1/(p+1/(2*p));
    if (x <= p){
        return A;
    }
    return A/p*x*exp(p*p-x*x);
}

//Working as expected
double G_inv_rej(const double x, const double p){
    const double A = 1/(p+1/(2*p));
    if (x <= A*p && x >= 0){ //New limit because change of coordinates. see notes
        return x/A;         //Returns maximum the value p, as decired
    } else if(x > A*p){
        // return sqrt(p*p - log(1-x*2*p/A));
        return sqrt(p*p - log(1+2*p*p - x*2*p/A));
    } else if(x < 0){
        cerr << "Wrong input x, should not go below 0";
    }
    return 0.;
}

vector<pair<double,double>> rejection_method(const int N, const double p){
    vector<pair<double,double>> sampled_values;
    vector<double> pdf_p;
    vector<double> G_vals;
    const double b = 100.;
    const double c = 1.5;
    while (sampled_values.size() < N){
        double x = ((double) rand() / RAND_MAX) * b; //Choose a range for x in [0,b]
        double eps = (double) rand() / RAND_MAX;
        double G_distributed = G_inv_rej(x, p); //The value X, which is distributed according to G
        G_vals.push_back(G_distributed);
        double test_value = c*g_rej(G_distributed, p);
        if (eps < p_rej(G_distributed)/(test_value)){
            sampled_values.push_back(pair<double,double>(G_distributed,eps*test_value)); 
            pdf_p.push_back(eps*test_value);    //Y_values, numbers distributed according to the pdf ish
        } 
    }
    string filepath = "./data/ex_2_3_1/";
    string filename_G = "G_vals_N=" + to_string(N) + "_p=" + to_string(p) + ".txt";
    string filename_pdf = "pdf_vals_N=" + to_string(N) + "_p=" + to_string(p) + ".txt";
    classic_write_to_file(G_vals, filepath, filename_G);
    return sampled_values;
}


//Ex 2.4

//Slowly varying function
double g(const double x){
    return sqrt(x); //Slowly varying for large x, goes towards convergancy
}

double f_i(const double x){
    return exp(-pow(x,2))*g(x);
}

double crude_method(const int N, const double a, const double b){
    double I_N = 0; //Integration sum
    for(int i=0; i < N; i++){
        double rv = a + ((double) rand() / RAND_MAX)*(b-a); //Picking a point uniformaly between a and b
        I_N += f_i(rv);
    }
    I_N *= (b-a)/N; //Adding (b-a)/N in the end to improve effiency
    return I_N;
}

//Reusing code from rejection task
double W_i(const double a, const double b){
    int i = 0;
    while(true){
        const double p = 0.8; //Found to be an okay value in rejection method task
        const double c = 1.5; //Found to be an okay value in rejection method task
        double x = a + ((double) rand() / RAND_MAX) * (b-a); //Choose a range for x in [a,b], usually a=0
        double eps = (double) rand() / RAND_MAX; //Value to check generated number up against
        double G_distributed = G_inv_rej(x, p); //The value X, which is distributed according to G
        double test_value = c*g_rej(G_distributed, p);
        if (eps < p_rej(G_distributed)/(test_value)){
            return eps*test_value;    //Y_values, numbers distributed according to the pdf ish
        }
        if (i > 10000){
            cerr << "Exceeded 10000 iterations, something may be wrong";
            return 0.;
        }
        i++;
    }
}

double Importanse_sampling_method(const int N, const double a, const double b){
    double I_N = 0;
    for(int i = 0; i < N; i++){
        double X = W_i(a, b); //Generating number according to the gaussian distribution from 0 to inf, but between the values a and b
        I_N += g(X);
    }
    I_N *= sqrt(M_PI)/(2*N); //Changed according to normalization
    return I_N;
}


//Ex cos

double G_inv_cos(const double x, const double a, const double b){
    const double c = a/b;
    return sqrt(2/b*x + c*c) - c;
}
double g_cos(const double x, const double a, const double b){
    return a+b*pow(x,2);
}

// Perform Monte Carlo integration with importance sampling
double cos_integration(const int N, const double a, const double b) {    
    double I_estimate = 0.0;

    for (int i = 0; i < N; i++) {
        double x = ((double) rand() / RAND_MAX) * M_PI/2; //Uniform distribution from 0 to pi/2
        double gen_number = G_inv_cos(x, a, b);
        // I_estimate += cos(gen_number)/G;
    }

    // integralEstimate *= (M_PI / 2.0) / N; // Scale by the range [0, π/2]

    return I_estimate;
}

void parameter_check(){
// Target accuracy
    double targetAccuracy = 0.01;
    
    // Initial parameter values
    double a = 1.0;
    double b = -0.4;
    int numSamples = 1000; // Start with a reasonable number of samples
    vector<double> errors{1};

    while (true) {
        double integralEstimate = cos_integration(a, b, numSamples);
        double error = std::abs(integralEstimate - 1.0); // The true integral is 1.0
        errors.push_back(error);
        std::cout << "a = " << a << ", b = " << b << ", Integral estimate = " << integralEstimate << ", Error = " << error << std::endl;

        if (error < targetAccuracy) {
            break;
        }

        // Adjust a and b based on the error
        // a += 0.01;
        if (abs(errors.at(errors.size() - 2) - error) > targetAccuracy*0.5){
            b -= 0.05;
            a += 0.01;
        } else{
            b += 0.01;
            a += 0.01;
        }
        
        // numSamples *= 2; // Double the number of samples in the next iteration
    }

    std::cout << "Optimal a = " << a << ", Optimal b = " << b << ", Required iterations = " << numSamples << std::endl;
}


//Variance and expectancy

//A minus values gives decireable values so far, with 0 < a <= 1
pair<double,double> a_star(const double T){
    const double a_1 = (2+2/T+sqrt(4*pow(T+1,2)/pow(T,2) - 8/T))/2;
    const double a_2 = (2+2/T-sqrt(4*pow(T+1,2)/pow(T,2) - 8/T))/2;
    return pair<double,double>{a_1,a_2};
}

double G_inv_var(double const a, double const rv){
    return -log(1-rv)/a;
}

int f_var(const double T, const double x){
    if (x < T){
        return 0;
    } else if (x >= T){
        return 1;
    }
    return 0;
}

double p_var(const double x){
    return exp(-x);
}

double P_inv_var(const double rv){
    return -log(1-rv);
}

double numerical_average(const double T, const int N){
    double av_sum = 0;
    for (int i = 0; i < N; i++){
        double rv = (double) rand() / RAND_MAX;
        double X = P_inv_var(rv);
        av_sum += f_var(T,X)*p_var(X);
    }
    av_sum *= 1./N;

    return av_sum;
}

double numerical_average_IS(const double T, const int N){
    double av_sum = 0;
    double a_str = a_star(T).second; //After some simulations, second option is the one in the a limits
    cout << "a* = " << a_str << endl;
    for (int i = 0; i < N; i++){
        double rv = (double) rand() / RAND_MAX;
        double X = G_inv_var(a_str, rv);
        double f_i = f_var(T, X)*p_var(X);
        double g_i = a_str*exp(-a_str*X);
        av_sum += f_i/g_i;
    }
    // cout << (G_inv_var(a_str, 0.99));
    // cout << "av before 1/N = " << av_sum << endl;l
    av_sum *= 1./N;

    return av_sum; 
}

double varians(const double average_value){
    return average_value*(1-average_value);
}

double weird_varians(const double a, const double T){
    return exp(-T*(2-a))/(a*(2-a)) - exp(-2*T);
}