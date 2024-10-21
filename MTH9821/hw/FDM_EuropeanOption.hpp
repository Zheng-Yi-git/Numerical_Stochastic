#ifndef FDM_EUROOPTION_HPP
#define FDM_EUROOPTION_HPP

#include "EuropeanOption.hpp"
#include <vector>

class FDMEuropean {
public:
    // Member variable to store the EuropeanOption object
    EuropeanOption option;
    double t;      // Current time
    double S;      // Current stock price
    double K;      // Strike price
    double T;      // Time to maturity
    double sigma;  // Volatility of the stock
    double r;      // Risk-free rate
    double q;      // Dividend yield
    double M;      // meshsize
    double alpha_temp;
    bool isPut;
    double a;
    double b;
    double x_left;
    double x_right;
    double Tau_final;
    double delta_Tau;
    int N;
    double delta_x;
    double alpha;
    std::vector<std::vector<double>> mesh;

    // Constructor
    FDMEuropean();
    FDMEuropean(const EuropeanOption& _option, double _M, double _alpha_temp, bool _isPut);

    // Destructor
    ~FDMEuropean();

    // 
    std::vector<std::vector<double>> Create_mesh();
    double f(double x);
    double g_left(double Tau);
    double g_right(double Tau);
};

#endif // FDM_EUROOPTION_HPP