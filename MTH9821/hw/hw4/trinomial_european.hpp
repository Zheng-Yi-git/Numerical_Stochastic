#ifndef TrinomialEuropean_hpp 
#define TrinomialEuropean_hpp

/*
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
*/
#include "black_scholes.hpp"
#include "trinomial_option.hpp"

#ifndef Option_value
#define Option_value
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
#endif

class TrinomialEuropean : public TrinomialOption
{
private:
    std::vector<std::string> methods{"Trinomial", "TBS", "TBSR"}; // KEEP THIS and below
    double S_temp;
    std::vector<double> V;
    double V10, V11, V20, V22, V12, V24;
    double price, delta1, gamma1, theta1;

public:
    TrinomialEuropean(double S0_, double K_, double r_, double sigma_, double T_, double q_, int N_, std::string method_, std::string type_)
        : TrinomialOption(S0_, K_, r_, sigma_, T_, q_, N_, method_, type_)
    {
        if (method == "Trinomial")
        {
            S_temp = S0 * pow(u, N);
        }
        else if (method == "TBS")
        {
            S_temp = S0 * pow(u, N - 1);
        }
        else if (method == "TBSR")
        {
            // do nothing
        }
        else
        {
            std::cout << "Method not found!" << std::endl;
        }
    }

    // getter (use const to avoid changing the member variables)
    double getPrice() const { return price; }
    double getDelta() const { return delta1; }
    double getGamma() const { return gamma1; }
    double getTheta() const { return theta1; }

    void runSimulation()
    {
        if (method == "Trinomial")
        {
            for (int i = 0; i <= 2 * N; i++)
            {
                V.push_back(option_value(K, S0 * pow(u, N - i), type));
            }
            for (int j = N - 1; j >= 0; j--)
            {
                if (j == 0)
                {
                    V10 = V[0];
                    V11 = V[1];
                    V12 = V[2];
                }
                if (j == 1)
                {
                    V20 = V[0];
                    V22 = V[2];
                    V24 = V[4];
                }
                for (int i = 0; i <= 2 * j; i++)
                {
                    V[i] = disc_p_u * V[i] + disc_p_m * V[i + 1] + disc_p_d * V[i + 2]; 
                }
            }
            // calculate price, delta, gamma, theta
            price = V[0];
            delta1 = (V10 - V12) / (S0 * (u - d));
            gamma1 = ((V20 - V22) / (S0 * u * (u - d)) - (V22 - V24) / (S0 * d * (u - d))) / (S0 * (u * u - d * d) / 2);
            theta1 = (V11 - price) / dt;
        }
        else if (method == "TBS")
        {
            for (int i = 0; i <= 2*N - 2; i++)
            {
                V.push_back(black_scholes_values(S0 * pow(u, N - i), K, dt, sigma, r, q)[type + "_price"]);
            }
            for (int j = N - 2; j >= 0; j--)
            {
                if (j == 0)
                {
                    V10 = V[0];
                    V11 = V[1];
                    V12 = V[2];
                }
                if (j == 1)
                {
                    V20 = V[0];
                    V22 = V[2];
                    V24 = V[4];
                }
                for (int i = 0; i <= 2 * j; i++)
                {
                    V[i] = disc_p_u * V[i] + disc_p_m * V[i + 1] + disc_p_d * V[i + 2];
                }
            }
            // calculate price, delta, gamma, theta
            price = V[0];
            delta1 = (V10 - V12) / (S0 * (u - d));
            gamma1 = ((V20 - V22) / (S0 * u * (u - d)) - (V22 - V24) / (S0 * d * (u - d))) / (S0 * (u * u - d * d) / 2);
            theta1 = (V11 - price) / dt;
        }
        else if (method == "TBSR")
        {
            TrinomialEuropean tree_N = TrinomialEuropean(S0, K, r, sigma, T, q, N, "TBS", type);
            TrinomialEuropean tree_halfN = TrinomialEuropean(S0, K, r, sigma, T, q, N / 2, "TBS", type);
            tree_N.runSimulation();
            tree_halfN.runSimulation();
            price = 2 * tree_N.getPrice() - tree_halfN.getPrice();
            delta1 = 2 * tree_N.getDelta() - tree_halfN.getDelta();
            gamma1 = 2 * tree_N.getGamma() - tree_halfN.getGamma();
            theta1 = 2 * tree_N.getTheta() - tree_halfN.getTheta();
        }

        else
        {
            std::cout << "Method not found!" << std::endl;
            std::cout << "Method tried: " << method << std::endl;
        }
    }
};

#endif