#ifndef BinomialEuropean_hpp
#define BinomialEuropean_hpp

#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include "black_scholes.hpp"

double option_value(double K, double S, std::string type)
{
    if (type == "call")
    {
        return std::max(0.0, S - K);
    }
    else if (type == "put")
    {
        return std::max(0.0, K - S);
    }
    else
    {
        std::cout << "Option type not found!" << std::endl;
    }
    return 0.0;
}

class BinomialEuropean
{
private:
    double S0, K, r, sigma, T, q;
    double dt;
    int N;
    double u, d, d_bar;
    double disc_p, disc_1p;
    double price;
    std::string type;
    std::string method;
    std::vector<std::string> methods{"Binomial", "AverageBinomial", "BBS", "BBSR"};
    double S_temp;
    std::vector<double> V;

public:
    BinomialEuropean(double S0_, double K_, double r_, double sigma_, double T_, double q_, int N_, std::string method_, std::string type_)
    {
        S0 = S0_;
        K = K_;
        r = r_;
        sigma = sigma_;
        T = T_;
        q = q_;
        N = N_;
        method = method_;
        type = type_;
        dt = T / N;
        u = exp(sigma * sqrt(dt));
        d = 1 / u;
        d_bar = pow(d, 2);
        disc_p = exp(-r * dt) * (exp((r - q) * dt) - d) / (u - d);
        disc_1p = exp(-r * dt) - disc_p;
        if (method == "Binomial")
        {
            S_temp = S0 * pow(u, N);
        }
        else if (method == "AverageBinomial")
        {
            // do nothing
        }
        else if (method == "BBS")
        {
            S_temp = S0 * pow(u, N - 1);
        }
        else if (method == "BBSR")
        {
            // do nothing
        }
        else
        {
            std::cout << "Method not found!" << std::endl;
        }
    }

    double getPrice()
    {
        if (method == "Binomial")
        {
            for (int i = 0; i <= N; i++)
            {
                V.push_back(option_value(K, S_temp, type));
                S_temp *= d_bar;
            }
            for (int j = N - 1; j >= 0; j--)
            {
                for (int i = 0; i <= j; i++)
                {
                    V[i] = disc_p * V[i] + disc_1p * V[i + 1];
                }
            }
            return V[0];
        }
        else if (method == "AverageBinomial")
        {
            BinomialEuropean tree_N = BinomialEuropean(S0, K, r, sigma, T, q, N, "Binomial", type);
            BinomialEuropean tree_Nplus1 = BinomialEuropean(S0, K, r, sigma, T, q, N + 1, "Binomial", type);
            return (tree_N.getPrice() + tree_Nplus1.getPrice()) / 2;
        }
        else if (method == "BBS")
        {
            for (int i = 0; i <= N - 1; i++)
            {
                V.push_back(black_scholes_values(S_temp, K, dt, sigma, r, q)[type + "_price"]);
                S_temp *= d_bar;
            }
            for (int j = N - 2; j >= 0; j--)
            {
                for (int i = 0; i <= j; i++)
                {
                    V[i] = disc_p * V[i] + disc_1p * V[i + 1];
                }
            }
            return V[0];
        }
        else if (method == "BBSR")
        {
            BinomialEuropean tree_N = BinomialEuropean(S0, K, r, sigma, T, q, N, "BBS", type);
            BinomialEuropean tree_halfN = BinomialEuropean(S0, K, r, sigma, T, q, N / 2, "BBS", type);
            return 2 * tree_N.getPrice() - tree_halfN.getPrice();
        }

        else
        {
            std::cout << "Method not found!" << std::endl;
        }
        return 0.0;
    }
};

#endif