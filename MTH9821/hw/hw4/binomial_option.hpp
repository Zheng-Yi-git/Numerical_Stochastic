#ifndef BinomialOption_hpp 
#define BinomialOption_hpp

#include <string>
#include <vector>
#include <cmath>
#include <iostream>

class BinomialOption {
protected:
    double S0, K, r, sigma, T, q;
    int N;
    std::string type;
    std::string method;
    double dt, u, d, d_bar, p, disc_p, disc_1p;

public:
    // Constructor
    BinomialOption(double S0_, double K_, double r_, double sigma_, double T_, double q_, int N_, std::string method_, std::string type_)
        : S0(S0_), K(K_), r(r_), sigma(sigma_), T(T_), q(q_), N(N_), method(method_), type(type_)
    {
        dt = T / N;
        u = exp(sigma * sqrt(dt));
        d = 1 / u;
        d_bar = pow(d, 2);
        p = (exp((r - q) * dt) - d) / (u - d);
        disc_p = exp(-r * dt) * p;
        disc_1p = exp(-r * dt) *(1-p);
    }

    // Virtual destructor
    virtual ~BinomialOption() {}

    // Abstract methods to be implemented by derived classes
    virtual void runSimulation() {};
    virtual double getPrice() const = 0;
    virtual double getDelta() const = 0;
    virtual double getGamma() const = 0;
    virtual double getTheta() const = 0;

    // Helper function for option value
    /*
    double option_value(double S, double K, std::string type) const {
        if (type == "call") {
            return std::max(0.0, S - K);
        } else if (type == "put") {
            return std::max(0.0, K - S);
        } else {
            std::cerr << "Option type not found!" << std::endl;
            return 0.0;
        }
    }
    */
};

#endif