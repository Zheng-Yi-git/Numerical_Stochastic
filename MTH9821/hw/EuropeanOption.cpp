#include "EuropeanOption.hpp"

EuropeanOption::EuropeanOption(){
t = 0;
S = 35.0;
K = 40.0;
T = 0.75;
sigma = 0.33;
r = 0.04;
q = 0.02; 
}

// Constructor implementation
EuropeanOption::EuropeanOption(double _t, double _S, double _K, double _T, double _sigma, double _r, double _q)
    : t(_t), S(_S), K(_K), T(_T), sigma(_sigma), r(_r), q(_q) {
}

// Destructor
EuropeanOption::~EuropeanOption() {
}