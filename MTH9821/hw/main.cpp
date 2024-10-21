#include <iostream>
#include "FDM_EuropeanOption.hpp"
#include <vector>

int main() {
    double t = 0;
    double sigma = 0.33;
    double S0 = 35;
    double q = 0.02;
    double K = 40;
    double T = 0.75;
    double r = 0.04;
    EuropeanOption option(t, S0, K, T, sigma, r, q);
    double alpha_temp = 0.45;
    double M = 4;
    FDMEuropean FDM(option, M, alpha_temp, true); // true because put option
    std::vector<std::vector<double>> mesh = FDM.mesh;
    for (int i=0; i < mesh.size(); i++){
        for (int j=0; j < mesh[0].size(); j++){
            std::cout << mesh[i][j] << "  ";
        }
        std::cout << std::endl;
    }
    return 0;
}