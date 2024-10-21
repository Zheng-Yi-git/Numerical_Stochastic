#include "FDM_EuropeanOption.hpp"

FDMEuropean::FDMEuropean() {
    t = 0.0;      // Current time
    S = 35.0;      // Current stock price
    K = 40.0;      // Strike price
    T = 0.75;      // Time to maturity
    sigma = 0.33;  // Volatility of the stock
    r = 0.04;      // Risk-free rate
    q = 0.02;      // Dividend yield
    M = 4;      // meshsize
    alpha_temp = 0.45;
    isPut = true;
    a = (r-q)/(sigma*sigma) - 0.5;
    b = ((r-q)/(sigma*sigma) + 0.5) * ((r-q)/(sigma*sigma) + 0.5) + 2*q/(sigma*sigma);
    x_left = std::log(S / K) + (r - q - 0.5 * sigma * sigma) * T - 3 * sigma * std::sqrt(T);
    x_right = std::log(S / K) + (r - q - 0.5 * sigma * sigma) * T + 3 * sigma * std::sqrt(T);
    Tau_final = 0.5 * T * sigma * sigma;
    delta_Tau = std::sqrt(Tau_final/M);
    N = std::floor((x_right-x_left)/std::sqrt(delta_Tau/alpha_temp));
    delta_x = (x_right-x_left)/N;
    alpha = delta_Tau/(delta_x * delta_x);
}

// Constructor implementation
FDMEuropean::FDMEuropean(const EuropeanOption& _option, double _M, double _alpha_temp, bool _isPut) {
    t = _option.t;      // Current time
    S = _option.S;      // Current stock price
    K = _option.K;      // Strike price
    T = _option.T;      // Time to maturity
    sigma = _option.sigma;  // Volatility of the stock
    r = _option.r;      // Risk-free rate
    q = _option.q;      // Dividend yield
    M = _M;      // meshsize
    alpha_temp = _alpha_temp;
    isPut = _isPut;
    a = (r-q)/(sigma*sigma) - 0.5;
    b = ((r-q)/(sigma*sigma) + 0.5) * ((r-q)/(sigma*sigma) + 0.5) + 2*q/(sigma*sigma);
    x_left = std::log(S / K) + (r - q - 0.5 * sigma * sigma) * T - 3 * sigma * std::sqrt(T);
    x_right = std::log(S / K) + (r - q - 0.5 * sigma * sigma) * T + 3 * sigma * std::sqrt(T);
    Tau_final = 0.5 * T * sigma * sigma;
    delta_Tau = std::sqrt(Tau_final/M);
    N = std::floor((x_right-x_left)/std::sqrt(delta_Tau/alpha_temp));
    delta_x = (x_right-x_left)/N;
    alpha = delta_Tau/(delta_x * delta_x);
    mesh = Create_mesh();
}

// Destructor
FDMEuropean::~FDMEuropean() {
}

std::vector<std::vector<double>> FDMEuropean::Create_mesh() {
    //double x_left = std::log(S / K) + (r - q - 0.5 * sigma * sigma) * T - 3 * sigma * std::sqrt(T);
    //double x_right = std::log(S / K) + (r - q - 0.5 * sigma * sigma) * T + 3 * sigma * std::sqrt(T);
    
    std::vector<std::vector<double>> mesh(M+1, std::vector<double>(N+1, 0.0));
    std::vector<double> U_prior(N+1, 0.0);
    std::vector<double> x_vec(N+1, 0.0);

    // Setup initial state
    for (int i = 0; i <= N; i++){
        x_vec[i] = x_left + i * delta_x;
        U_prior[i] = f(x_vec[i]);
    }
    mesh[0] = U_prior;
    std::vector<double> U_posterior(N+1, 0.0);

    // Generate next row of mesh
    for (int j = 1; j <= M; j++) {
        for (int i = 1; i < N; i++){
            U_posterior[i] = alpha * U_prior[i-1] + (1-2*alpha) * U_prior[i] + alpha * U_prior[i+1];
        U_posterior[0] = g_left(j*delta_Tau); // Not entirely sure if j*delta_Tau is correct here for input to g_left
        U_posterior[N] = g_right(j*delta_Tau); // Not entirely sure if j*delta_Tau is correct here for input to g_right
        // U_posterior[0] = alpha * g_left(j*delta_Tau) + (1-2*alpha) * U_prior[0] + alpha * U_prior[1]; // Not entirely sure if j*delta_Tau is correct here for input to g_left
        // U_posterior[N] = alpha * U_prior[N-1] + (1-2*alpha) * U_prior[N] + alpha * g_right(j*delta_Tau); // Not entirely sure if j*delta_Tau is correct here for input to g_right
        mesh[j] = U_posterior;
        U_prior = U_posterior;
        }
    }
    return mesh;
}

double FDMEuropean::f(double x) {
    if (isPut){
        return K * std::exp(a*x) * std::max(1-std::exp(x),0.0);
    }
    else{
        return K * std::exp(a*x) * std::max(std::exp(x)-1,0.0);
    }
}

double FDMEuropean::g_left(double Tau) {
    if (isPut){
        return K * std::exp(a*x_left + b*Tau) * (std::exp(-2*r*Tau/(sigma*sigma)) - std::exp(x_left-2*q*Tau/(sigma*sigma)));
    }
    else{
        return 0.0;
    }
}

double FDMEuropean::g_right(double Tau) {
    if (isPut){
        return 0.0;
    }
    else{
        return K * std::exp(a*x_right + b*Tau) * (std::exp(x_right-2*r*Tau/(sigma*sigma)) - std::exp(-2*r*Tau/(sigma*sigma)));
    }
}