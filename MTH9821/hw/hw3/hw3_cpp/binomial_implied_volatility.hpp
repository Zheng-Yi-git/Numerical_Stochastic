
#pragma once

#include "black_scholes.hpp"
#include "binomial_option.hpp"
#include "binomial_european.hpp"

double calculateOptionPrice_BBSR(double sigma, double S0, double K, double r, double T, double q, int N, const std::string& type) {
    BinomialEuropean option(S0, K, r, sigma, T, q, N, "BBSR", type);  // Use the finest binomial tree model - BBSR
    option.runSimulation();
    return option.getPrice();
}

// Find proper N_fixed with tolerance 1e-4
int find_N_fixed(double sigma, double S0, double K, double r, double T, double q, const std::string& type, double tol = 1e-4) {
    int N = 10;  // initial time steps
    double price_prev = calculateOptionPrice_BBSR(sigma, S0, K, r, T, q, N, type);  // Initial price
    double price_curr = 0.0;

    std::cout << "Finding N_fixed...\n";
    std::cout << "N" << std::setw(30) << "Option Price (BBSR)\n";
    std::cout << N << std::setw(30) << price_prev << std::endl;

    while (true) {
        N *= 2;  // Double the time steps
        price_curr = calculateOptionPrice_BBSR(sigma, S0, K, r, T, q, N, type);  // calculate new option price

        std::cout << N << std::setw(30) << price_curr << std::endl;

        // If the difference between two prices within tolerance 1e-4, stop iteration
        if (std::fabs(price_curr - price_prev) < tol) {
            std::cout << "N_fixed found: " << N << std::endl;
            return N;
        }

        // Update price, continue iteration
        price_prev = price_curr;
    }
}

double impliedVolatility_BBSR(double marketPrice, double S0, double K, double r, double T, double q, int N, const std::string& type, double tol = 1e-4, int max_iter = 100) {
    double sigma0 = 0.05;  // initial volatility lower bound
    double sigma1 = 1;  // initial volatility upper bound
    double price0 = calculateOptionPrice_BBSR(sigma0, S0, K, r, T, q, N, type);
    double price1 = calculateOptionPrice_BBSR(sigma1, S0, K, r, T, q, N, type);

    std::cout << "Iterative vol value" << std::setw(20) << "Option value (BBSR)\n";
    std::cout << sigma0 << std::setw(30) << price0 << std::endl;
    std::cout << sigma1 << std::setw(30) << price1 << std::endl;

    int iter = 0;
    while (iter < max_iter) {
        double sigma2 = sigma1 - (price1 - marketPrice) * (sigma1 - sigma0) / (price1 - price0);
        double price2 = calculateOptionPrice_BBSR(sigma2, S0, K, r, T, q, N, type);

        std::cout << sigma2 << std::setw(30) << price2 << std::endl;

        if (std::fabs(price2 - marketPrice) < tol) {
            std::cout << "Implied volatility found: " << sigma2 << std::endl;
            return sigma2;  // Volatility converges
        }

        sigma0 = sigma1;
        price0 = price1;
        sigma1 = sigma2;
        price1 = price2;

        iter++;
    }

    std::cerr << "Failed to converge in " << max_iter << " iterations." << std::endl;
    return sigma1;  // return the final result
}